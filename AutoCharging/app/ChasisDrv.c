#include "stm32f10x.h"
#include "ChasisDrv.h"
#include "mymath.h"

//坐标系定义 判断跟ROS坐标系定义是否吻合 吻合为1 不吻合为-1
#define  FORWARD_POSITIVE_FACTOR			(1)
#define  ANGULAR_POSITIVE_FACTOR			(1)


RobotPose robotPose;
RobotPose globalPose;
RobotSpdState robotSpd;
RobotModel    robotModel;


//速度为m/s 转换为rpm
void MotorMileToRevolution(double *spd)
{
	//速度单位为m/s 转换为r/min
	spd[0] = spd[0] / (PI * WheelDiameter) * 60 * MotorReduction;
	spd[1] = spd[1] / (PI * WheelDiameter) * 60 * MotorReduction;
	spd[2] = spd[2] / (PI * WheelDiameter) * 60 * MotorReduction;
	
}

double vx_factor = 0.7142;  //前进速度校正因子，保证实际速度和给点速度相差不大。
double vtheta_factor = 0.71875;
void Robot_MoveOmni(double vx,double vy,double vtheta)
{
	double spd[3];
	double v1,v2,v3;
	
	vx *= FORWARD_POSITIVE_FACTOR;
	//vtheta *= ANGULAR_POSITIVE_FACTOR;
	
	vx  = vx * vx_factor;
	vtheta = vtheta * vtheta_factor;
	
	vtheta = - vtheta;
	
	vx /= 1000;
	vy /= 1000;
	
	/*三角底盘运动学模型 轮1的垂直方向为X轴*/
	spd[0] =          0*vx +         1*vy + WheelD*vtheta;
  spd[1] = -sin(PI/3)*vx - cos(PI/3)*vy + WheelD*vtheta;
  spd[2] =  sin(PI/3)*vx - cos(PI/3)*vy + WheelD*vtheta;
	
	MotorMileToRevolution(spd);
	//MotorSetRunVelocity(spd[0],spd[1],spd[2]);
	//Motor_setRun(spd[0],spd[1],spd[2]);
}


//把差分轮车体的速度，转换为三个轮子的速度

void RobotBodyToWheels(double vx,double vy,double vtheta,double *v)
{
	vx *= FORWARD_POSITIVE_FACTOR;
	vtheta *= ANGULAR_POSITIVE_FACTOR;
	
	vx = vx * vx_factor;
	vtheta = vtheta * vtheta_factor;
	
	v[0] =          0*vx +         1*vy + WheelD*vtheta;
  v[1] = -sin(PI/3)*vx - cos(PI/3)*vy + WheelD*vtheta;
  v[2] =  sin(PI/3)*vx - cos(PI/3)*vy + WheelD*vtheta;
}


/*
把三角底盘的轮子编码器的值转换为两轮差分的编码器的值
*/
void robotReportEncoder(double v1,double v2,double v3)
{
	double dl,dtheta;
	double vx,vy,vtheta;
	double vl,vr;
	double temp_theta;
	
	/*把编码器的值转换距离单位m*/
	v1 = v1 / EncoderTicks * PI * WheelDiameter;
	v2 = v2 / EncoderTicks * PI * WheelDiameter;
	v3 = v3 / EncoderTicks * PI * WheelDiameter;
	
	/*正运动学模型*/
	vx = 				 (-2.0/3.0)*0*v1 + 			(-sqrt33*1)*v2 + 				 (sqrt33)*v3;
  vy =  					(2.0/3.0)*v1 + 			 (-(1.0/3))*v2 + 			 (-(1.0/3))*v3;
  vtheta = (1.0/(3*WheelD))*v1 + (1.0/(3*WheelD))*v2 + (1.0/(3*WheelD))*v3;
	
	vx *= FORWARD_POSITIVE_FACTOR;
	vtheta *= ANGULAR_POSITIVE_FACTOR;
	
  /*转换为mm 和 角度 */
	vx *= 1000;
	vy *= 1000;
	vtheta *= 59.2957;
	vtheta =-vtheta;
	
	robotPose.x += vx;
	robotPose.y += vy;
	robotPose.theta += vtheta;
	
	globalPose.x += vx;
	globalPose.y += vy;
	globalPose.theta += vtheta;
	
	/*转换为前进距离和*/
	dl = sqrt(vx*vx + vy*vy) /1000.0 *Sign(vx);
	dtheta = AngleToRad(vtheta);
	
	//转换为左右两轮走过的距离 单位为m
	vr = dl + 0.6 * dtheta / 2;
	vl = dl - 0.6 * dtheta / 2;
	
	//mm
	vl *= 1000;
	vr *= 1000;
	
	robotPose.lspd = vl;
	robotPose.rspd = vr;
	
	robotPose.ldistance += vl;
	robotPose.rdistance += vr;
}


//把全向轮的3个速度转换为车子的前进速度 m/s
double RobotGetForwardVelocity(double v1,double v2,double v3,double dt,double *vx,double *vtheta)
{
	double vy,v;
	v1 = v1 / EncoderTicks * PI * WheelDiameter;
	v2 = v2 / EncoderTicks * PI * WheelDiameter;
	v3 = v3 / EncoderTicks * PI * WheelDiameter;
	
	/*正运动学模型*/
	*vx =   			(-2.0/3.0)*0*v1 + 			 (-sqrt33)*v2 + 				(sqrt33)*v3;
  vy =    			 (2.0/3.0)*v1 + 			(-(1.0/3))*v2 + 			(-(1.0/3))*v3;
  *vtheta =(1.0/(3*WheelD))*v1 + (1.0/(3*WheelD))*v2 + (1.0/(3*WheelD))*v3;
	
	*vx *= FORWARD_POSITIVE_FACTOR;
	*vtheta *= ANGULAR_POSITIVE_FACTOR;
	
	*vtheta = *vtheta / dt;
	
	/*平移距离*/
	v = sqrt((*vx)*(*vx) + vy*vy) * Sign(*vx);
	
	/*平移速度*/
	v = v / dt ;
	
	return v;
}

double RobotGetForwardPosition(double v1,double v2,double v3)
{
	double pos_x,pos_y,pos;
	//m/s
	v1 = v1 / EncoderTicks * PI * WheelDiameter;
	v2 = v2 / EncoderTicks * PI * WheelDiameter;
	v3 = v3 / EncoderTicks * PI * WheelDiameter;
	
	pos_x =   			(-2.0/3.0)*0*v1 + 			 (-sqrt33)*v2 + 				(sqrt33)*v3;
  pos_y =    			 (2.0/3.0)*v1 + 			(-(1.0/3))*v2 + 			(-(1.0/3))*v3;
	
	pos_x *= FORWARD_POSITIVE_FACTOR;
	
	pos = sqrt(pos_x * pos_x + pos_y*pos_y) * Sign(pos_x);
	return pos;
	
}


//计算出车子的旋转角速度单位弧度 /s
double RobotGetAngularVelocity(double angular,double dt)
{
	static double lastangular = 0;
	double vel = 0;
	
	vel = (angular - lastangular);
	
	if(vel > PI)
		vel -= 2*PI;
	else if(vel < -PI)
		vel += 2*PI;
	
	vel = vel / dt;
	
	lastangular = angular;
	return vel;
}


//坐标清零
void RobotClearStatus()
{
	robotPose.ldistance = 0;
	robotPose.rdistance = 0;
}














