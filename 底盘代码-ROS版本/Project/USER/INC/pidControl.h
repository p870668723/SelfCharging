#ifndef PID_CONTROL_H
#define PID_CONTROL_H

#ifdef __cplusplus
 extern "C" {
#endif


#include "string.h"
#include "mymath.h"

typedef struct sdlfjsdlkf
{
	double kp,ki,kd;
	double err[2];
	double sumErr;
	double maxOut;
	double maxIout;
	double pOut,iOut,dOut,Out;
	double setValue;
	double thresholdi;
}PID_Struct;

extern PID_Struct pid_forward,pid_angular;

float PID_Calc(PID_Struct * pid,float nowValue);
void PID_setTargetValue(PID_Struct *pid,float value);
void PID_ClearSumError(PID_Struct *pid);
void PID_Init(void );


#ifdef __cplusplus
}
#endif

#endif

