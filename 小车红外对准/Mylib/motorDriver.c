#include "motorDriver.h"
#include "mymath.h"

void MotorDir_Init(void )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin= GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE,&GPIO_InitStructure);
	
}

void MotorEnable_Init(void )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin= GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOI,&GPIO_InitStructure);
	
}

void MotorPWM_Init(void )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOI,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOI,&GPIO_InitStructure);
	
	GPIO_PinAFConfig(GPIOI,GPIO_PinSource5,GPIO_AF_TIM8);
	GPIO_PinAFConfig(GPIOI,GPIO_PinSource6,GPIO_AF_TIM8);
	GPIO_PinAFConfig(GPIOI,GPIO_PinSource7,GPIO_AF_TIM8);
	
	TIM_TimeBaseInitStructure.TIM_Prescaler = 0;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period  = PWM_Period;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM8,&TIM_TimeBaseInitStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set;
	
	TIM_OC1Init(TIM8,&TIM_OCInitStructure);
	TIM_OC2Init(TIM8,&TIM_OCInitStructure);
	TIM_OC3Init(TIM8,&TIM_OCInitStructure);
	
	TIM_OC1PreloadConfig(TIM8,TIM_OCPreload_Enable);
	TIM_OC2PreloadConfig(TIM8,TIM_OCPreload_Enable);
	TIM_OC3PreloadConfig(TIM8,TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM8,ENABLE);
	TIM_CtrlPWMOutputs(TIM8,ENABLE);
	
	TIM_Cmd(TIM8,ENABLE);
	
}




void Motor_setDuty(int mNum,float duty)
{
	float value;
	
	/*
	驱动器：10%代表高速 90%代表低速
	程序：  10%代表低速 90%代表高速
	所以需要取反
	*/
	if(duty > 0) duty = 100 - duty;
	else if(duty < 0) duty = -100 - duty;
	
	value = PWM_Period / 100.0 * duty;
	if(mNum == 1)
	{
		if(value < 0)
		{
			MotorOne_setNeg();
			TIM8->CCR1 = -(int)value;
			return ;
		}
		MotorOne_setPos();
		TIM8->CCR1 = value;
	}
	else if(mNum == 2)
	{
		if(value < 0)
		{
			MotorTwo_setNeg();
			TIM8->CCR2 = -(int)value;
			return ;
		}
		MotorTwo_setPos();
		TIM8->CCR2 = value;
	}
	else if(mNum == 3)
	{
		if(value < 0)
		{
			MotorThree_setNeg();
			TIM8->CCR3 = -(int)value;
			return ;
		}
		MotorThree_setPos();
		TIM8->CCR3 = value;
	}
}

void Motor_setRun(double duty1,double duty2,double duty3 )
{
	//duty的单位为r/min
	//转换为占空比

  if(__fabs(duty1 - 0) < 1e-5) duty1 = 0;
	else
  {
		if(duty1 > 0)
		  duty1 = duty1 / MAX_SPEED * 80;
		else duty1 = duty1 / MAX_SPEED * 80;
	}
	
	if(__fabs(duty2 - 0) < 1e-5) duty2 = 0;
	else
  {
		if(duty2 > 0)
		 duty2 = duty2 / MAX_SPEED * 80;
		else duty2 = duty2 / MAX_SPEED * 80;
		
	}
	
	if(__fabs(duty3 - 0) < 1e-5) duty3 = 0;
	else
  {
		if(duty3 > 0)
		 duty3 = duty3 / MAX_SPEED * 80;
		else duty3 = duty3 / MAX_SPEED * 80;
	}
	
	duty1 += 0*Sign(duty1);
	duty2 += 0*Sign(duty2);
	duty3 += 0*Sign(duty3);
	
	Motor_setDuty(1,duty1);
	Motor_setDuty(2,duty2);
	Motor_setDuty(3,duty3);
}

void Motor_setDutys(double *duty)
{
	Motor_setDuty(1,duty[0]);
	Motor_setDuty(2,duty[1]);
	Motor_setDuty(3,duty[2]);
}

//释放电机
void Motor_Release(void )
{
	MotorOne_Disable();
	MotorTwo_Disable();
	MotorThree_Disable();
}

void Motor_Enable(void )
{
	MotorOne_Enable();
	MotorTwo_Enable();
	MotorThree_Enable();
}



void Motor_Init(void )
{
	MotorDir_Init();
	MotorEnable_Init();
	MotorPWM_Init();
	
	Motor_Enable();
}








