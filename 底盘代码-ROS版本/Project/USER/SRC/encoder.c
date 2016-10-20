#include "encoder.h"


void WheelOneEncoder(void )
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	GPIO_InitTypeDef        GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(EncoderOne_TIM_RCC,ENABLE);
	RCC_AHB1PeriphClockCmd(EncoderOne_GPIO_RCC,ENABLE);

	GPIO_PinAFConfig(EncoderOne_GPIO,EncoderOne_GPIO_PinsourceA,EncoderOne_TIM_AF);
	GPIO_PinAFConfig(EncoderOne_GPIO,EncoderOne_GPIO_PinsourceB,EncoderOne_TIM_AF);
	
	GPIO_InitStructure.GPIO_Pin= EncoderOne_GPIO_BITs;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_Init(EncoderOne_GPIO,&GPIO_InitStructure);
	
	TIM_TimeBaseInitStructure.TIM_Period=65535;   
	TIM_TimeBaseInitStructure.TIM_Prescaler=0;  
	TIM_TimeBaseInitStructure.TIM_ClockDivision=0;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(EncoderOne_TIM,&TIM_TimeBaseInitStructure);
	
	TIM_EncoderInterfaceConfig(EncoderOne_TIM,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);	
	
	TIM_SetCounter(EncoderOne_TIM,0x7fff);
	TIM_Cmd(EncoderOne_TIM,ENABLE);
}

void WheelTwoEncoder(void )
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	GPIO_InitTypeDef        GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(EncoderTwo_TIM_RCC,ENABLE);
	RCC_AHB1PeriphClockCmd(EncoderTwo_GPIO_RCC,ENABLE);

	GPIO_PinAFConfig(EncoderTwo_GPIO,EncoderTwo_GPIO_PinsourceA,EncoderTwo_TIM_AF);
	GPIO_PinAFConfig(EncoderTwo_GPIO,EncoderTwo_GPIO_PinsourceB,EncoderTwo_TIM_AF);
	
	GPIO_InitStructure.GPIO_Pin= EncoderTwo_GPIO_BITs;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_Init(EncoderTwo_GPIO,&GPIO_InitStructure);
	
	TIM_TimeBaseInitStructure.TIM_Period=65535;   
	TIM_TimeBaseInitStructure.TIM_Prescaler=0;  
	TIM_TimeBaseInitStructure.TIM_ClockDivision=0;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(EncoderTwo_TIM,&TIM_TimeBaseInitStructure);
	
	TIM_EncoderInterfaceConfig(EncoderTwo_TIM,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);	
	
	TIM_SetCounter(EncoderTwo_TIM,0x7fff);
	TIM_Cmd(EncoderTwo_TIM,ENABLE);
}

void WheelThreeEncoder(void )
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	GPIO_InitTypeDef        GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(EncoderThree_TIM_RCC,ENABLE);
	RCC_AHB1PeriphClockCmd(EncoderThree_GPIO_RCC,ENABLE);

	GPIO_PinAFConfig(EncoderThree_GPIO,EncoderThree_GPIO_PinsourceA,EncoderThree_TIM_AF);
	GPIO_PinAFConfig(EncoderThree_GPIO,EncoderThree_GPIO_PinsourceB,EncoderThree_TIM_AF);
	
	GPIO_InitStructure.GPIO_Pin= EncoderThree_GPIO_BITs;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_Init(EncoderThree_GPIO,&GPIO_InitStructure);
	
	TIM_TimeBaseInitStructure.TIM_Period=65535;   
	TIM_TimeBaseInitStructure.TIM_Prescaler=0;  
	TIM_TimeBaseInitStructure.TIM_ClockDivision=0;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInit(EncoderThree_TIM,&TIM_TimeBaseInitStructure);
	
	TIM_EncoderInterfaceConfig(EncoderThree_TIM,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);	
	
	TIM_SetCounter(EncoderThree_TIM,0x7fff);
	TIM_Cmd(EncoderThree_TIM,ENABLE);
}


void WheelEncoder_Init(void )
{
	WheelOneEncoder();
	WheelTwoEncoder();
	WheelThreeEncoder();
}






