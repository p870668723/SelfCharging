#ifndef APP_H_
#define APP_H_

#include "stm32f10x.h"
void delay_us(u16 nus);
u16 Get_Adc(u8 ch);
u16 Get_Adc_Average(u8 ch,u8 times);

#endif