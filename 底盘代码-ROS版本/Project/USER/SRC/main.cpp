/****************************************************************************************
小车电机编号：屁股后面的电机编号为1,按逆时针顺序电机的编号分别为1 2 3
每个电机的逆时针旋转方向为正方向。
当电机都正转时，旋转方向为顺时针旋转。与一般的定义正好是相反的。

在ROS中，机器人的坐标系定义为下：
X轴指向前进方向，Y轴指向左边，Z轴指向上边。
旋转方向：逆时针旋转方向为正。


根据本身坐标系和ROS机器人坐标系的定义。
他们两个的角度的方向是反的，所以小车往ROS里发送指令的时候，需要把角度取反。


****************************************************************************************/

#include "oled.h"
#include "board.h"
#include "motorDriver.h"
#include "robotmodel.h"
#include "encoder.h"
#include "timer.h"
#include "bluetooth.h"

#include "uart.h"	 
#include "common.h"
#include "servo.h"
#include "I2C.h"
#include "ks103.h"
#include "systick.h"
#include "dbus.h"
#include "avoidobstacle.h"
#include "MPU6050.h"
#include "imu.h"
#include "kalman.h"
#include "dbus.h"
#include "pidControl.h"
#include "can1.h"
#include "usart1.h"



double tmp_gyro[3],tmp_acc[3];
int cnt = 0;
void main_Init()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		 //配置中断
	Timer_Init();  																 		 //定时器 OK
	OLED_Init();  																  	 // OK
	CAN1_Configuration();															 //CAN1 电机
	Board_Init();  																	   //beep ok
	MPU6050_Init();                                    //MPU6050初始化
	IMU_Init();																			   //IMU_Init
	USART2_Init();																		 //串口2初始化  用来向上位机报告数据
//	Bluetooth_Init();																   //bluetooth OK
//	
//	PID_Init();
	
	//复位所有驱动器
	systick_ms(1000);
	CAN_RoboModule_DRV_Reset(0,0);
  systick_ms(1000);
	
	//选择模式
	CAN_RoboModule_DRV_Mode_Choice(0,1,PWM_VELOCITY_MODE);
	CAN_RoboModule_DRV_Mode_Choice(0,2,PWM_VELOCITY_MODE);
	CAN_RoboModule_DRV_Mode_Choice(0,3,PWM_VELOCITY_MODE);
  systick_ms(500);	
	
	//配置为返回编码器线数
	CAN_RoboModule_DRV_Config(0,1,1,0);
	CAN_RoboModule_DRV_Config(0,2,1,0);
	CAN_RoboModule_DRV_Config(0,3,1,0);
	
	systick_ms(100);
	
	DBUS_Init();
	
}

int v1,v2,v3;   					//三个轮子编码器的值
int vsum = 0;
double lastYaw,nowYaw = 0;
int   urm_counter = 0;
double vx_show,vtheta_show;
const int dt_ms = 5;
const int report_period = 50;

double target_forward_pos;
double target_angular_pos;
double now_forward_pos;
double now_angular_pos;
double last_target_forward_pos;
double last_target_angular_pos;

double forward_vel;
double angular_vel;
double pid_forward_adjust;
double pid_angular_adjust;
double vvv,ttt;
double final_vx,final_vtheta;
int lost_cnt = 0;

double v[3];

RobotEncoder tmp_robot_encoder;

double gyro_show = 0;

int main(void )
{
	main_Init();
	StartSystemTime();
	
	while(1)
	{
		urm_counter ++;
		systick_ms(dt_ms);
		
		UpdateSystemTime();    //更新系统时间
		Robot_MoveOmni(target_vx,0,target_vtheta);
		gyro_show = IMU_Cal() * 57.295f;
		
		if(urm_counter >= (report_period / dt_ms))
		{
			/*在SLAM的情况下，进行心跳检测*/
			if(Control_Mode == SLAM_MODE)
			{
				heartbreak_cnt ++;
				if(heartbreak_cnt > 5)  //很长一段时间没有收到指令，则赋值为0  250ms
				{
					target_vx = 0;
					target_vtheta = 0;
					lost_cnt ++;
				}
			}
			urm_counter = 0;
			
			/*读取编码器的数值*/
			//锁住
			
			//关闭中断
			NVIC_DisableIRQ(CAN1_RX0_IRQn);
			robot_encoder_data.islock = 1;
			
			/*计算两次发送的时间内的增量*/
		  tmp_robot_encoder = robot_encoder_data;
			
			robot_encoder_data.prev1 = robot_encoder_data.v1;
			robot_encoder_data.prev2 = robot_encoder_data.v2;
			robot_encoder_data.prev3 = robot_encoder_data.v3;
			
			robot_encoder_data.islock = 0;
			NVIC_EnableIRQ(CAN1_RX0_IRQn);
			
			tmp_robot_encoder.delta_v1 = tmp_robot_encoder.v1 - tmp_robot_encoder.prev1;
			tmp_robot_encoder.delta_v2 = tmp_robot_encoder.v2 - tmp_robot_encoder.prev2;
			tmp_robot_encoder.delta_v3 = tmp_robot_encoder.v3 - tmp_robot_encoder.prev3;
			
			robotModel.v1 = tmp_robot_encoder.delta_v1;
			robotModel.v2 = tmp_robot_encoder.delta_v2;
			robotModel.v3 = tmp_robot_encoder.delta_v3;
			//解锁
			//打开中断
			
			Yaw = IMU_GetYawRadian();
			OLED_Printf(0,0,"Yaw:%.2f",Yaw*57.3);
			
			//当前角速度
			forward_vel = RobotGetForwardVelocity(robotModel.v1,robotModel.v2,robotModel.v3,report_period / 1000.0,&vvv,&ttt);
			angular_vel =	RobotGetAngularVelocity(Yaw, report_period / 1000.0);
		//	OLED_Printf(0,0,"VX:%.2f",forward_vel);
		
			 //OLED_Printf(0,0,"TX:%d",(int)(target_vx));
		   OLED_Printf(1,0,"VX:%d",(int)(forward_vel*1000));
			 OLED_Printf(2,0,"VT:%.2f",angular_vel*57.295);
			 OLED_Printf(3,0,"TT:%.2f",-target_vtheta*57.295);
			
//			//最终速度为期望速度加上修正速度
//			final_vx = target_vx + pid_forward_adjust;
//			final_vtheta = target_vtheta + pid_angular_adjust;
//			
//			//转换为3个轮子的速度
//			RobotBodyToWheels(final_vx,0,final_vtheta,v);
//			
//			//m/s 转换为 rpm
//			MotorMileToRevolution(v);
//			//把控制命令发送给驱动器。
//			MotorSetRunVelocity(v[0],v[1],v[2]);
//			
//			/*下面向上位机汇报自己的工作状态*/
			  robotModel.angle_vel = Yaw;       //汇报绝对角度
//			
//			//向上位机汇报信息
				Robot_Report(&robotModel);
				robotModel.v1 = robotModel.v2 = robotModel.v3 = 0; //clear
				
//			OLED_Printf(0,0,"Yaw: %.2f",Yaw);
//			OLED_Printf(1,0,"VX;%.2f",angular_vel);
//			OLED_Printf(2,0,"GV:%.2f",gyro_show);
//			OLED_Printf(3,0,"TV:%.2f",now_angular_pos);
//			
//			/*显示小车运行模式---调试使用*/
			if(Control_Mode == SLAM_MODE)
			{
				OLED_Printf(3,0,"SLAM");
			}
			else if(Control_Mode == REMOTE_MODE)
			{
				OLED_Printf(3,0,"REMOTE");
			}
		}      
	}
	return 0;
}
