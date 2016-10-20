#ifndef APP_H_
#define APP_H_

#include "stm32f10x.h"


extern s32 uOffset_AD0,uOffset_AD1,uOffset_AD2,uOffset_AD3;
extern s32 ureal_irda_data0,ureal_irda_data1,ureal_irda_data2,ureal_irda_data3;
extern s32 sreal_irda_Position;


void delay_us(u16 nus);
u16 Get_Adc(u8 ch);
u16 Get_Adc_Average(u8 ch,u8 times);


#endif