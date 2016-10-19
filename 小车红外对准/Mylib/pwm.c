#include "pwm.h"
/*************************************************************************
                              PWM初始化       
*************************************************************************/
void PWM_GPIO_Config()
{

    GPIO_InitTypeDef GPIO_InitStruct;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);  
    GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_11;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB,&GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);

}
/*************************************************************************
                              PWM初始化       
*************************************************************************/
void PWM_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    TIM_BDTRInitTypeDef TIM_BDTRInitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 | RCC_APB2Periph_AFIO, ENABLE);

    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 5000;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 4;

    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

    /* Channel 1 Configuration in PWM mode */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
//    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
//    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
//    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;

    TIM_OC1Init(TIM1, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM1, TIM_OCPreload_Enable);

    TIM_OC4Init(TIM1, &TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM1, TIM_OCPreload_Enable);

    /* Main Output Enable */
    TIM_CtrlPWMOutputs(TIM1, ENABLE);
 
    TIM_Cmd(TIM1, ENABLE); 
}

void Moto_Init(void)
{
  PWM_GPIO_Config();
  PWM_Configuration();
}
#define MOTOR_OUT_DEAD_VAL 500
float Motor_Oot_DEAD(float f_tVal)
{
     if(f_tVal > 0) 
 f_tVal += MOTOR_OUT_DEAD_VAL;
 else if(f_tVal < 0) 
 f_tVal -= MOTOR_OUT_DEAD_VAL;
 
      if(f_tVal>=4999) //输出限幅
     f_tVal=4999;
     else  if(f_tVal<=-4999)
     f_tVal=-4999;
 return f_tVal;
}

/******************************************************************************
 * 函数名称: SetMotorVoltage
 * 函数功能: 输入信号解析
 * 入口参数: float f_tVal
 ******************************************************************************/
void Set_L_Motor_Voltage(float f_tVal)
{

    f_tVal=Motor_Oot_DEAD( f_tVal);
	if(f_tVal>0)
	{		
	TIM1->CCR1=f_tVal;
	GPIO_ResetBits(GPIOB,GPIO_Pin_8);
	}
	else
	{
	TIM1->CCR1=5000+f_tVal;
    GPIO_SetBits(GPIOB,GPIO_Pin_8);
	}
}

void Set_R_Motor_Voltage(float f_tVal)
{
    f_tVal=Motor_Oot_DEAD( f_tVal);
	if(f_tVal>0)
	{		
	TIM1->CCR4=f_tVal;
	GPIO_ResetBits(GPIOB,GPIO_Pin_11);
	}
	else
	{
	TIM1->CCR4=5000+f_tVal;
    GPIO_SetBits(GPIOB,GPIO_Pin_11);
	}
}
