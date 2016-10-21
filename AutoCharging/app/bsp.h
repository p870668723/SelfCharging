#ifndef BSP_H_
#define BSP_H_

#include "stm32f10x.h"


#define LED_RON_GOFF {GPIO_SetBits(GPIOA,GPIO_Pin_7);GPIO_ResetBits(GPIOA,GPIO_Pin_6);}
#define LED_ROFF_GON {GPIO_SetBits(GPIOA,GPIO_Pin_6);GPIO_ResetBits(GPIOA,GPIO_Pin_7);}

extern CanTxMsg TxMessage;
extern CanRxMsg RxMessage;
extern int ChargeFlag;//充电标志，通过can的中断函数对其进行置位

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
void USART1_Configuration(void);

void Can_Sent(u8* data);
#endif
/***************************end of file*****************************************/

