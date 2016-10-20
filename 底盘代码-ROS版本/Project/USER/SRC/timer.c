#include "timer.h"

float dist_forward;
float dist_left;
float dist_right;
double system_time = 0;


int delayValue = 5;
int delayflag = 0;
int delayCounter = 0;
void Timer_Config(void )
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef        NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7,ENABLE);
	
	/* TIM7 Clocked at 84 MHz  1ms = 1000*/
	TIM_TimeBaseStructInit(&TIM_TimeBaseInitStructure);      //不加这句，跟串口一起使用的时候，会引起冲突，导致定时不准
	TIM_TimeBaseInitStructure.TIM_Period = 1000;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 84 - 1;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM7,&TIM_TimeBaseInitStructure);
	
	
	TIM_ITConfig(TIM7,TIM_IT_Update,ENABLE);
	TIM_ClearFlag(TIM7,TIM_FLAG_Update);
	
	TIM_Cmd(TIM7,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}

void SystemTime_Config(void )
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6,ENABLE);
	
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 84-1;    										//1M
	TIM_TimeBaseStructure.TIM_Period = 65535;													//65ms
	TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;  									//
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 			//向上计数
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);	
	
	/*预装载*/
	TIM_ARRPreloadConfig(TIM6, ENABLE);
	
	/*失能定时器*/
	TIM_Cmd(TIM6, ENABLE);
}


int GetSystemTime(void )
{
	return system_time ;
}


//更新系统时间 用在主循环里面。
void UpdateSystemTime(void )
{
	static int last_time = 0;
	int time = 0;
	
	time = TIM6->CNT;
	if(time >= last_time)
	{
		system_time += time - last_time;
	}
	else if(time < last_time)
	{
		system_time += time - last_time + 0xffff;
	}
	last_time = time;
}

void StartSystemTime(void )
{
	TIM6->CNT = 0;
	system_time = 0;
}

void Timer_Init(void )
{
//	Timer_Config();
	SystemTime_Config();
}


int millis(void )
{
	return delayCounter;
}


void TIM7_IRQHandler(void )
{
	static int counter;
	counter ++;
	delayCounter++;
	if(counter >= delayValue)
	{
		counter = 0;
		delayflag = 1;
	}
	TIM_ClearFlag(TIM7,TIM_FLAG_Update);
}








