#include "avoidobstacle.h"
#include "mymath.h"
#include "bluetooth.h"

//三角函数值
#define Sin287		0.4802
#define Cos287		0.8771

#define FORCE_FACTOR   400

double sonar[3];


void SonarAvoidObstacle(double *sonarData,double *spd)
{
	//由于障碍物所产生的推力速度
	double vx,vy,vtheta;
	double v1,v2,v3;
	double l1,l2,l3;
	
//	if(sonarData[0] > 500)
//	{
//		
//	}
	
	//
	l1 = My_Min(sonarData[0],700);
	l2 = My_Min(sonarData[1],700);
	l3 = My_Min(sonarData[2],700);
	
	l1 += 1;
	l2 += 1;
	l3 += 1;
	
	if(l1 < 100+250)
	{
		v1 = FORCE_FACTOR;
	}
	else if(l1 < 200+250)
	{
		v1 = FORCE_FACTOR * 0.8;
	}
	else if(l1 < 300+250)
	{
		v1 = FORCE_FACTOR * 0.6;
	}
	else if(l1 < 400+250)
	{
		v1 = FORCE_FACTOR * 0.3;
	}
	else if(l1 < 500+250)
	{
		v1 = FORCE_FACTOR * 0.1;
	}
	else v1 = 0;
	
	
	if(l2 < 100+150)
	{
		v2 = FORCE_FACTOR;
	}
	else if(l2 < 200+150)
	{
		v2 = FORCE_FACTOR * 0.8;
	}
	else if(l2 < 300+150)
	{
		v2 = FORCE_FACTOR * 0.6;
	}
	else if(l2 < 400+150)
	{
		v2 = FORCE_FACTOR * 0.3;
	}
	else if(l2 < 500+150)
	{
		v2 = FORCE_FACTOR * 0.1;
	}
	else v2 = 0;
	
	
	if(l3 < 100+250)
	{
		v3 = FORCE_FACTOR;
	}
	else if(l3 < 200+250)
	{
		v3 = FORCE_FACTOR * 0.8;
	}
	else if(l3 < 300+250)
	{
		v3 = FORCE_FACTOR * 0.6;
	}
	else if(l3 < 400+250)
	{
		v3 = FORCE_FACTOR * 0.3;
	}
	else if(l3 < 500+250)
	{
		v3 = FORCE_FACTOR * 0.1;
	}
	else v3 = 0;
	
	
	vx = -v2 - v1*Cos287 * 0.4 - v3*Cos287 * 0.4;
	vx *= 0.25;
	vy = -v1*Sin287 + v3*Sin287;
	vy *= 0.7;
	vtheta = 0;
	
	vx = -vx;
	
	Bluetooth_Printf(USART3,"vx:%.1f  vy:%.1f   %.1f %.1f %.1f \r\n",vx,vy,l1,l2,l3);
	
	
 /*三角底盘运动学模型 轮1的垂直方向为X轴*/
	spd[0] +=          0*vx +         1*vy + WheelD*vtheta;
  spd[1] += -sin(PI/3)*vx - cos(PI/3)*vy + WheelD*vtheta;
  spd[2] +=  sin(PI/3)*vx - cos(PI/3)*vy + WheelD*vtheta;
	
}










