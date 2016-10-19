#ifndef __PWM_H__
#define __PWM_H__
#include "stm32f10x.h"
void Moto_Init(void);
void Set_L_Motor_Voltage(float f_tVal);
void Set_R_Motor_Voltage(float f_tVal);
#endif
