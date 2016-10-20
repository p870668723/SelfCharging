/****************************************************************************************
С�������ţ�ƨ�ɺ���ĵ�����Ϊ1,����ʱ��˳�����ı�ŷֱ�Ϊ1 2 3
ÿ���������ʱ����ת����Ϊ������
���������תʱ����ת����Ϊ˳ʱ����ת����һ��Ķ����������෴�ġ�

��ROS�У������˵�����ϵ����Ϊ�£�
X��ָ��ǰ������Y��ָ����ߣ�Z��ָ���ϱߡ�
��ת������ʱ����ת����Ϊ����


���ݱ�������ϵ��ROS����������ϵ�Ķ��塣
���������ĽǶȵķ����Ƿ��ģ�����С����ROS�﷢��ָ���ʱ����Ҫ�ѽǶ�ȡ����


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
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		 //�����ж�
	Timer_Init();  																 		 //��ʱ�� OK
	OLED_Init();  																  	 // OK
	CAN1_Configuration();															 //CAN1 ���
	Board_Init();  																	   //beep ok
	MPU6050_Init();                                    //MPU6050��ʼ��
	IMU_Init();																			   //IMU_Init
	USART2_Init();																		 //����2��ʼ��  ��������λ����������
//	Bluetooth_Init();																   //bluetooth OK
//	
//	PID_Init();
	
	//��λ����������
	systick_ms(1000);
	CAN_RoboModule_DRV_Reset(0,0);
  systick_ms(1000);
	
	//ѡ��ģʽ
	CAN_RoboModule_DRV_Mode_Choice(0,1,PWM_VELOCITY_MODE);
	CAN_RoboModule_DRV_Mode_Choice(0,2,PWM_VELOCITY_MODE);
	CAN_RoboModule_DRV_Mode_Choice(0,3,PWM_VELOCITY_MODE);
  systick_ms(500);	
	
	//����Ϊ���ر���������
	CAN_RoboModule_DRV_Config(0,1,1,0);
	CAN_RoboModule_DRV_Config(0,2,1,0);
	CAN_RoboModule_DRV_Config(0,3,1,0);
	
	systick_ms(100);
	
	DBUS_Init();
	
}

int v1,v2,v3;   					//�������ӱ�������ֵ
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
		
		UpdateSystemTime();    //����ϵͳʱ��
		Robot_MoveOmni(target_vx,0,target_vtheta);
		gyro_show = IMU_Cal() * 57.295f;
		
		if(urm_counter >= (report_period / dt_ms))
		{
			/*��SLAM������£������������*/
			if(Control_Mode == SLAM_MODE)
			{
				heartbreak_cnt ++;
				if(heartbreak_cnt > 5)  //�ܳ�һ��ʱ��û���յ�ָ���ֵΪ0  250ms
				{
					target_vx = 0;
					target_vtheta = 0;
					lost_cnt ++;
				}
			}
			urm_counter = 0;
			
			/*��ȡ����������ֵ*/
			//��ס
			
			//�ر��ж�
			NVIC_DisableIRQ(CAN1_RX0_IRQn);
			robot_encoder_data.islock = 1;
			
			/*�������η��͵�ʱ���ڵ�����*/
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
			//����
			//���ж�
			
			Yaw = IMU_GetYawRadian();
			OLED_Printf(0,0,"Yaw:%.2f",Yaw*57.3);
			
			//��ǰ���ٶ�
			forward_vel = RobotGetForwardVelocity(robotModel.v1,robotModel.v2,robotModel.v3,report_period / 1000.0,&vvv,&ttt);
			angular_vel =	RobotGetAngularVelocity(Yaw, report_period / 1000.0);
		//	OLED_Printf(0,0,"VX:%.2f",forward_vel);
		
			 //OLED_Printf(0,0,"TX:%d",(int)(target_vx));
		   OLED_Printf(1,0,"VX:%d",(int)(forward_vel*1000));
			 OLED_Printf(2,0,"VT:%.2f",angular_vel*57.295);
			 OLED_Printf(3,0,"TT:%.2f",-target_vtheta*57.295);
			
//			//�����ٶ�Ϊ�����ٶȼ��������ٶ�
//			final_vx = target_vx + pid_forward_adjust;
//			final_vtheta = target_vtheta + pid_angular_adjust;
//			
//			//ת��Ϊ3�����ӵ��ٶ�
//			RobotBodyToWheels(final_vx,0,final_vtheta,v);
//			
//			//m/s ת��Ϊ rpm
//			MotorMileToRevolution(v);
//			//�ѿ�������͸���������
//			MotorSetRunVelocity(v[0],v[1],v[2]);
//			
//			/*��������λ���㱨�Լ��Ĺ���״̬*/
			  robotModel.angle_vel = Yaw;       //�㱨���ԽǶ�
//			
//			//����λ���㱨��Ϣ
				Robot_Report(&robotModel);
				robotModel.v1 = robotModel.v2 = robotModel.v3 = 0; //clear
				
//			OLED_Printf(0,0,"Yaw: %.2f",Yaw);
//			OLED_Printf(1,0,"VX;%.2f",angular_vel);
//			OLED_Printf(2,0,"GV:%.2f",gyro_show);
//			OLED_Printf(3,0,"TV:%.2f",now_angular_pos);
//			
//			/*��ʾС������ģʽ---����ʹ��*/
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
