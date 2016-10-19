#ifndef __ENCODER_H__
#define __ENCODER_H__
#include "stm32f10x.h"
void Encoder_Configuration(void);
void Encoder_R_Start(void);
void Encoder_L_Start(void);
int Encoder_Get_L_CNT(void);
int Encoder_Get_R_CNT(void);
typedef struct {
 s16 Encoder_V;
 int Encoder_L;
} S_Moto;
extern S_Moto S_Moto_L,S_Moto_R;
#endif
