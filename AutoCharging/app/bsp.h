#ifndef BSP_H_
#define BSP_H_

#include "stm32f10x.h"

extern CanTxMsg TxMessage;
extern CanRxMsg RxMessage;

void BSP_Init(void);
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void CAN1_Configuration(void);
void IIC_Configuration(void);
//void 	Encoder_Configuration();
void DMA_Configuration(void);
void PWM_Configuration(void);
void ADC_Configuration(void);

void Can_Sent(u8* data);
#endif
/***************************end of file*****************************************/

