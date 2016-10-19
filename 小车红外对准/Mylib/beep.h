#ifndef __BEEP_H__
#define __BEEP_H__
#include "stm32f10x.h"
#define BEEP_ON()        GPIO_SetBits(GPIOA,GPIO_Pin_4)
#define BEEP_OFF()       GPIO_ResetBits(GPIOA,GPIO_Pin_4)
#define BEEP_TOGGLE()    GPIOA->ODR ^= GPIO_Pin_4

void BEEP_Configuration(void);

#endif
