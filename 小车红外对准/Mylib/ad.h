#ifndef __AD_H__
#define __AD_H__
#include "stm32f10x.h"
void ADC1_Configuration(void);
void ADC3_Configuration(void);
int ADC_Filter(void);
int ADC3_Filter(void);
extern u16 ADC1_ConvertedValue[32];
extern u16 ADC3_ConvertedValue[32];
extern s32 ureal_irda_data1,ureal_irda_data2;
extern s32 sreal_irda_Position;
void offset_ad();
void get_irda_data();
#endif
