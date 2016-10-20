#include "misc.h"
#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"
#include "myos.h"
#include "can1.h"
#include "oled.h"
#include "board.h"
#include "motorDriver.h"
#include "robotmodel.h"
#include "encoder.h"
#include "timer.h"


//#include "serial_channel.h"
//#include "uart.h"	 
//#include "interchip.h"
//#include "common.h"

RCC_ClocksTypeDef RR;

void SlamTec_Init(void )
{
	
}

void main_Init()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //≈‰÷√÷–∂œ
	Timer_Init();   //∂® ±∆˜
	CAN1_Init();
	OLED_Init();   // OK
	Board_Init();  //beep ok
	Motor_Init();
	WheelEncoder_Init();
	RCC_GetClocksFreq(&RR);
}







void mainDelay(int t)
{
	while(t--)
	{
		int a = 1000;
		while(a--);
	}	
}

int main(void )
{
	int i1 = 0,	i2 = 0,i3 = 0;
	int temp1,temp2,temp3;
	main_Init();
	
	
	delayflag = 0;
	while(1)   
	{   
		if(delayflag)
		{
			delayflag = 0;
			EncoderOne_GetEncoder(temp1);
			EncoderTwo_GetEncoder(temp2);
			EncoderThree_GetEncoder(temp3);
		  i1 += temp1;
			i2 ++;
		}
		OLED_Printf(0,0,"E:%.1f %d",temp1/400.0,i2);
		OLED_Printf(1,0,"E:%.1f",temp2/400.0);
		OLED_Printf(2,0,"E:%.1f",temp3/400.0);
		
	}
	return 0;
}
