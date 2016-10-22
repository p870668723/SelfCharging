#include "stm32f10x.h"
#include "bsp.h"
#include "I2C.h"
#include "app.h"
#include "timer.h"
#include "mpu6050.h"
#include "ChasisDrv.h"

int fputc(int ch, FILE *f)
{
  /* Place your implementation of fputc here */
  USART_SendData(USART1, (u8)ch);
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
  return ch;
}

int main(void)
{
	u8 data[8]={1,2,3,4,5,6,7,8};  //for test...
	BSP_Init();
	Timer_Init();  																 		 //定时器 OK
//	MPU6050_Init();                                    //MPU6050初始
	systick_ms(1000);

//	if(RxMessage.StdId==0x100)
//	{
//		a=RxMessage.Data[0];
//		
//	}
	
	/*测试*/
	while(1)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_6 | GPIO_Pin_7);
		systick_ms(1000);
	  GPIO_ResetBits(GPIOA,GPIO_Pin_6 | GPIO_Pin_7);
		systick_ms(1000);
		Can_Sent(data);
	}
	
	/*轮询控制*/
	while(1) 
	{
//		printf("Usart Testing... \n");
//		LED_ROFF_GON;
		if(ChargeFlag==1)
		{
			//开始充电，接管底盘的运动控制
			get_irda_data();
			if(sreal_irda_Position>0)//向右偏
			{
				//LED_RON_GOFF;
				Robot_MoveOmni( 0,0,1);
			}
			else if(sreal_irda_Position<0)//向左偏
			{
				//LED_RON_GOFF;
				Robot_MoveOmni( 0,0,-1);
			}
			else//已对准
			{
				//LED_ROFF_GON;
				Robot_MoveOmni( 0,1,0);
			}
		}
		else 
		{
			LED_ROFF_GON;
			Robot_MoveOmni( 0,0,1);
			//结束充电，
		}
	}
}
