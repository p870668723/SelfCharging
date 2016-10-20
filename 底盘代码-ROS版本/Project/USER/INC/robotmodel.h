#ifndef ROBOT_MODEL_H
#define ROBOT_MODEL_H

//C文件里面的函数 要在C++文件里面引用，就需要加入extern "C"
#ifdef __cplusplus
extern "C" {
#endif


#include <math.h>
#include "motorDriver.h"
#include "mymath.h"

	

#define WheelD					0.26          //轮子离车中心的距离
#define EncoderTicks  	32000         //转一圈编码器的线数
#define WheelDiameter 	0.127         //轮子的直径
#define MotorReduction 	16            //电机减速比

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


void MotorMileToRevolution(double *spd);									//电机从m/s 转换为rpm
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


