#ifndef TIMER_H
#define TIMER_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f10x.h"
#include "stm32f10x_tim.h"

void UpdateSystemTime(void );
void Timer_Init(void );
void StartSystemTime(void );
int GetSystemTime(void );
int millis(void );
extern int delayflag,delayCounter;

extern float dist_forward,dist_left,dist_right;

#ifdef __cplusplus
}
#endif


#endif

