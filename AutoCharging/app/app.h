#ifndef APP_H_
#define APP_H_

#include "stm32f10x.h"


extern s32 uOffset_AD0,uOffset_AD1,uOffset_AD2,uOffset_AD3;
extern s32 ureal_irda_data0,ureal_irda_data1,ureal_irda_data2,ureal_irda_data3;
extern s32 sreal_irda_Position;
extern int ChargeFlag;

void delay_us(u16 nus);
u16 Get_Adc(u8 ch);
u16 Get_Adc_Average(u8 ch,u8 times);
void MotorSetRunVelocity(short rpm1,short rpm2,short rpm3);
void CAN_RoboModule_DRV_PWM_Velocity_Mode(unsigned char Group,unsigned char Number,short Temp_PWM,short Temp_Velocity);
void offset_ad(void);
void get_irda_data(void);
#endif
