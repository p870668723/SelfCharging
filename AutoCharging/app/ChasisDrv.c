#include "stm32f10x.h"
#include "ChasisDrv.h"
#include "mymath.h"

//����ϵ���� �жϸ�ROS����ϵ�����Ƿ��Ǻ� �Ǻ�Ϊ1 ���Ǻ�Ϊ-1
#define  FORWARD_POSITIVE_FACTOR			(1)
#define  ANGULAR_POSITIVE_FACTOR			(1)


RobotPose robotPose;
RobotPose globalPose;
RobotSpdState robotSpd;
RobotModel    robotModel;


//�ٶ�Ϊm/s ת��Ϊrpm
void MotorMileToRevolution(double *spd)
{
	//�ٶȵ�λΪm/s ת��Ϊr/min
	spd[0] = spd[0] / (PI * WheelDiameter) * 60 * MotorReduction;
	spd[1] = spd[1] / (PI * WheelDiameter) * 60 * MotorReduction;
	spd[2] = spd[2] / (PI * WheelDiameter) * 60 * MotorReduction;
	
}

double vx_factor = 0.7142;  //ǰ���ٶ�У�����ӣ���֤ʵ���ٶȺ͸����ٶ�����
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
	
	/*���ǵ����˶�ѧģ�� ��1�Ĵ�ֱ����ΪX��*/
	spd[0] =          0*vx +         1*vy + WheelD*vtheta;
  spd[1] = -sin(PI/3)*vx - cos(PI/3)*vy + WheelD*vtheta;
  spd[2] =  sin(PI/3)*vx - cos(PI/3)*vy + WheelD*vtheta;
	
	MotorMileToRevolution(spd);
	//MotorSetRunVelocity(spd[0],spd[1],spd[2]);
	//Motor_setRun(spd[0],spd[1],spd[2]);
}


//�Ѳ���ֳ�����ٶȣ�ת��Ϊ�������ӵ��ٶ�

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
�����ǵ��̵����ӱ�������ֵת��Ϊ���ֲ�ֵı�������ֵ
*/
void robotReportEncoder(double v1,double v2,double v3)
{
	double dl,dtheta;
	double vx,vy,vtheta;
	double vl,vr;
	double temp_theta;
	
	/*�ѱ�������ֵת�����뵥λm*/
	v1 = v1 / EncoderTicks * PI * WheelDiameter;
	v2 = v2 / EncoderTicks * PI * WheelDiameter;
	v3 = v3 / EncoderTicks * PI * WheelDiameter;
	
	/*���˶�ѧģ��*/
	vx = 				 (-2.0/3.0)*0*v1 + 			(-sqrt33*1)*v2 + 				 (sqrt33)*v3;
  vy =  					(2.0/3.0)*v1 + 			 (-(1.0/3))*v2 + 			 (-(1.0/3))*v3;
  vtheta = (1.0/(3*WheelD))*v1 + (1.0/(3*WheelD))*v2 + (1.0/(3*WheelD))*v3;
	
	vx *= FORWARD_POSITIVE_FACTOR;
	vtheta *= ANGULAR_POSITIVE_FACTOR;
	
  /*ת��Ϊmm �� �Ƕ� */
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
	
	/*ת��Ϊǰ�������*/
	dl = sqrt(vx*vx + vy*vy) /1000.0 *Sign(vx);
	dtheta = AngleToRad(vtheta);
	
	//ת��Ϊ���������߹��ľ��� ��λΪm
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


//��ȫ���ֵ�3���ٶ�ת��Ϊ���ӵ�ǰ���ٶ� m/s
double RobotGetForwardVelocity(double v1,double v2,double v3,double dt,double *vx,double *vtheta)
{
	double vy,v;
	v1 = v1 / EncoderTicks * PI * WheelDiameter;
	v2 = v2 / EncoderTicks * PI * WheelDiameter;
	v3 = v3 / EncoderTicks * PI * WheelDiameter;
	
	/*���˶�ѧģ��*/
	*vx =   			(-2.0/3.0)*0*v1 + 			 (-sqrt33)*v2 + 				(sqrt33)*v3;
  vy =    			 (2.0/3.0)*v1 + 			(-(1.0/3))*v2 + 			(-(1.0/3))*v3;
  *vtheta =(1.0/(3*WheelD))*v1 + (1.0/(3*WheelD))*v2 + (1.0/(3*WheelD))*v3;
	
	*vx *= FORWARD_POSITIVE_FACTOR;
	*vtheta *= ANGULAR_POSITIVE_FACTOR;
	
	*vtheta = *vtheta / dt;
	
	/*ƽ�ƾ���*/
	v = sqrt((*vx)*(*vx) + vy*vy) * Sign(*vx);
	
	/*ƽ���ٶ�*/
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


//��������ӵ���ת���ٶȵ�λ���� /s
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


//��������
void RobotClearStatus()
{
	robotPose.ldistance = 0;
	robotPose.rdistance = 0;
}














