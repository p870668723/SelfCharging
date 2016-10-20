#include "pidControl.h"

PID_Struct pid_forward,pid_angular;

float iii = 300;

/***************************************************************
函数名称：void PID_Init(void )
函数功能：PID参数初始化
***************************************************************/
void PID_Init(void )
{
	pid_forward.kp = 5;
	pid_forward.ki = 0.5;
	pid_forward.kd = 0.4;
	pid_forward.maxOut = 2;
	pid_forward.maxIout = 1;
	pid_forward.setValue = 0;

	
	pid_angular.kp = 10;
	pid_angular.ki = 1;
	pid_angular.kd = 0.0;
	pid_angular.maxOut = 2;
	pid_angular.maxIout = 1;
	pid_angular.setValue = 0;
	pid_angular.thresholdi = 10;
	
}

/***************************************************************
函数名称：void PID_setParam(float kp,float ki,float kd)
函数功能：PID参数设置
***************************************************************/
void PID_setParam(PID_Struct * pid,float kp,float ki,float kd)
{
	pid->kp = kp;
	pid->ki = ki;
	pid->kd = kd;
	
}

void PID_ClearSumError(PID_Struct *pid)
{
	pid->sumErr = 0;
}


void PID_setTargetValue(PID_Struct *pid,float value)
{
	pid->setValue = value;
}

/***************************************************************
函数名称：float PID_Calc(PID_Struct * pid,float setValue,float nowValue)
函数功能：PID计算
***************************************************************/
float PID_Calc(PID_Struct * pid,float nowValue)
{
	
	pid->err[1] = pid->setValue - nowValue;
	pid->pOut = pid->err[1] * pid->kp;

	pid->dOut = (pid->err[1] - pid->err[0]) * pid->kd;

	if(pid->ki != 0.0f)
	{
		pid->sumErr += pid->err[1];
		if(MyAbs(pid->sumErr) > pid->thresholdi )
		{
			pid->sumErr = pid->thresholdi * Sign(pid->sumErr);
		}
	  pid->iOut = pid->sumErr * pid->ki;
		
		pid->iOut = My_Min(MyAbs(pid->iOut),pid->maxIout) * Sign(pid->iOut);
	}
	else
	{
		pid->sumErr += 0;
		pid->iOut = 0;
	}
	
	pid->Out = pid->pOut + pid->iOut + pid->dOut;
	
	
	if(pid->Out > pid->maxOut)
		pid->Out = pid->maxOut;
	else if(pid->Out < - pid->maxOut)
		pid->Out = - pid->maxOut;
	
	return pid->Out;
}









