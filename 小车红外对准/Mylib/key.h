#ifndef __KEY_H__
#define __KEY_H__
#include "stm32f10x.h"
#define Key0  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)
#define Key1  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15)
void KEY_Configuration(void);
#endif
