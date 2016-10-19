#include "key.h"

void KEY_Configuration(void)
{
    GPIO_InitTypeDef gpio;  
//    EXTI_InitTypeDef exti;
//    NVIC_InitTypeDef nvic;
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOC, ENABLE);    
   // GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
    
	gpio.GPIO_Pin = GPIO_Pin_15;	
	gpio.GPIO_Mode = GPIO_Mode_IPU;
	gpio.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &gpio);
    
    gpio.GPIO_Pin = GPIO_Pin_5;	
	gpio.GPIO_Mode = GPIO_Mode_IPU;
	gpio.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOC, &gpio);

}

