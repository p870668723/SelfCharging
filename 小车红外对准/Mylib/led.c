#include "main.h"

//LED_GREEN----PA6, '0' is ON,'1' is OFF
//LED_RED------PA7, '0' is ON,'1' is OFF

void LED_Configuration(void)
{
  GPIO_InitTypeDef gpio;   

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD , ENABLE);
		
	gpio.GPIO_Pin = GPIO_Pin_2;	
	gpio.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &gpio);

}
