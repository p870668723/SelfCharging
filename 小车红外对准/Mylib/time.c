#include "main.h"
#include "stm32f10x.h"
u32 Go_Time;
void TIM6_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  tim;
    NVIC_InitTypeDef         nvic;

    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);

    //nvic.NVIC_IRQChannel = TIM6_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);

    tim.TIM_Prescaler = 71;
    tim.TIM_CounterMode = TIM_CounterMode_Up;
    tim.TIM_ClockDivision = TIM_CKD_DIV1;
    tim.TIM_Period = 5000;
    TIM_TimeBaseInit(TIM6,&tim);
    TIM6_Start();
}

void TIM7_Configuration(void)//50hz
{
    TIM_TimeBaseInitTypeDef  tim;
    NVIC_InitTypeDef         nvic;

    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);

    //nvic.NVIC_IRQChannel = TIM7_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);

    tim.TIM_Prescaler = 720-1;
    tim.TIM_CounterMode = TIM_CounterMode_Up;
    tim.TIM_ClockDivision = TIM_CKD_DIV1;
    tim.TIM_Period = 2000;
    TIM_TimeBaseInit(TIM7,&tim);
    TIM_Cmd(TIM7, ENABLE);
    TIM_ITConfig(TIM7, TIM_IT_Update,ENABLE);
    TIM_ClearFlag(TIM7, TIM_FLAG_Update);
}
void TIM6_Start(void)
{
    TIM_Cmd(TIM6, ENABLE);
    TIM_ITConfig(TIM6, TIM_IT_Update,ENABLE);
    TIM_ClearFlag(TIM6, TIM_FLAG_Update);	
}
void TIM7_IRQHandler(void)  
{
    if (TIM_GetITStatus(TIM7,TIM_IT_Update)!= RESET) 
	{       
            Go_Time++;
            S_Moto_L.Encoder_V=Encoder_Get_L_CNT();
            S_Moto_R.Encoder_V=Encoder_Get_R_CNT();
            S_Moto_L.Encoder_L+=S_Moto_L.Encoder_V;
            S_Moto_R.Encoder_L+=S_Moto_R.Encoder_V;
        TIM_ClearITPendingBit(TIM7,TIM_IT_Update);
		TIM_ClearFlag(TIM7, TIM_FLAG_Update); 
    }
}
void TIM6_IRQHandler(void)  
{
    if (TIM_GetITStatus(TIM6,TIM_IT_Update)!= RESET) 
	{

           if(Go_Mode==GO)
            Moto_Ctrl();
        //  MPU6050_Angle.Yaw-=MPU6050_Real_Data.Gyro_Z/180*9/10;
        TIM_ClearITPendingBit(TIM6,TIM_IT_Update);
		TIM_ClearFlag(TIM6, TIM_FLAG_Update);  
  }
}
