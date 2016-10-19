#include "encoder.h"

void Encoder_Configuration(void)
{

    GPIO_InitTypeDef gpio;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM4,ENABLE);     
    gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
    gpio.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA,&gpio);
    
    gpio.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
    gpio.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB,&gpio);
    
    TIM_EncoderInterfaceConfig(TIM2, 
	                            TIM_EncoderMode_TI12,
							                TIM_ICPolarity_Falling,
							                TIM_ICPolarity_Falling);
    TIM_EncoderInterfaceConfig(TIM4, 
	                            TIM_EncoderMode_TI12,
							                TIM_ICPolarity_Falling,
							                TIM_ICPolarity_Falling);                                            
    TIM_Cmd(TIM2,ENABLE);
    TIM_Cmd(TIM4,ENABLE);
    Encoder_L_Start();
    Encoder_R_Start();
}

void Encoder_L_Start(void)
{
    TIM2->CNT = 0x7fff;
}
void Encoder_R_Start(void)
{
    TIM4->CNT = 0x7fff;
}
int Encoder_Get_L_CNT(void)
{
    int cnt = 0;
    cnt = (TIM2->CNT)-0x7fff;
    TIM2->CNT = 0x7fff;
    return cnt;
}
int Encoder_Get_R_CNT(void)
{
    int cnt = 0;
    cnt = (TIM4->CNT)-0x7fff;
    TIM4->CNT = 0x7fff;
    return cnt;
}
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2,TIM_IT_Update)!= RESET) 
	{
	   TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
  } 
}
void TIM4_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM4,TIM_IT_Update)!= RESET) 
	{
	   TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
  } 
}
