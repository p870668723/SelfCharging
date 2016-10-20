#ifndef ROBOT_MODEL_H
#define ROBOT_MODEL_H

//C�ļ�����ĺ��� Ҫ��C++�ļ��������ã�����Ҫ����extern "C"
#ifdef __cplusplus
extern "C" {
#endif


#include <math.h>
#include "motorDriver.h"
#include "mymath.h"

	

#define WheelD					0.26          //�����복���ĵľ���
#define EncoderTicks  	32000         //תһȦ������������
#define WheelDiameter 	0.127         //���ӵ�ֱ��
#define MotorReduction 	16            //������ٱ�

#define sqrt33        	0.577350     //sqrt(3)/3


typedef struct ddd
{
	double lspd,rspd;
	double x,y,theta;
	double ldistance,rdistance;
}RobotPose;



typedef struct sdddddddd
{
	double lspd,rspd;
}RobotSpd;


typedef struct sss
{
	RobotSpd target;
	RobotSpd now;
	double dl,dr;
}RobotSpdState;


typedef struct aaaa
{
	double v1,v2,v3;
	double angle_vel;
}RobotModel;

extern RobotPose robotPose;
extern RobotPose globalPose;
extern RobotSpdState robotSpd;
extern RobotModel    robotModel;


void MotorMileToRevolution(double *spd);									//�����m/s ת��Ϊrpm
void Robot_Move(double lspd,double rspd,double *spd);
void robotReportEncoder(double v1,double v2,double v3);
void Robot_MoveOmni(double vx,double vy,double vtheta);
void robotReport(RobotModel *robot);

void RobotBodyToWheels(double vx,double vy,double vtheta,double *v);
void MotorRunVelocity(double * spd);

double RobotGetForwardVelocity(double v1,double v2,double v3,double dt,double *vx,double *vtheta);
double RobotGetForwardPosition(double v1,double v2,double v3);
double RobotGetAngularVelocity(double angular,double dt);


#ifdef __cplusplus
}
#endif

#endif


