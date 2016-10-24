#include "stm32f10x.h"
#include "bsp.h"
#include "I2C.h"
#include "app.h"
#include "timer.h"
#include "mpu6050.h"
#include "ChasisDrv.h"
#include <stdio.h>

int fputc(int ch, FILE *f)
{
  /* Place your implementation of fputc here */
  USART_SendData(USART1, (u8)ch);
  while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
  return ch;
}

int main(void)
{
	u8 data[8]={2,2,3,3,4,4,5,5};  //for test...
	BSP_Init();
	Timer_Init();  																 		 //��ʱ�� OK
//	MPU6050_Init();                                    //MPU6050��ʼ
//	systick_ms(1000);

	/*����*/
	while(1)
	{
		if(ChargeFlag==1)
		{
			GPIO_SetBits(GPIOA,GPIO_Pin_6 | GPIO_Pin_7);
			systick_ms(200);
			GPIO_ResetBits(GPIOA,GPIO_Pin_6 | GPIO_Pin_7);
			systick_ms(200);
		}
		else
		{
			GPIO_SetBits(GPIOA,GPIO_Pin_6 | GPIO_Pin_7);
			systick_ms(600);
			GPIO_ResetBits(GPIOA,GPIO_Pin_6 | GPIO_Pin_7);
			systick_ms(600);
		}
		
		Can_Sent(data);
		printf("Usart Testing... \n");
	}
	
	/*��ѯ����*/
	while(1) 
	{
//		printf("Usart Testing... \n");
//		LED_ROFF_GON;
		if(ChargeFlag==1)
		{
			//��ʼ��磬�ӹܵ��̵��˶�����
			get_irda_data();
			if(sreal_irda_Position>0)//����ƫ
			{
				//LED_RON_GOFF;
				Robot_MoveOmni( 0,0,1);
			}
			else if(sreal_irda_Position<0)//����ƫ
			{
				//LED_RON_GOFF;
				Robot_MoveOmni( 0,0,-1);
			}
			else//�Ѷ�׼
			{
				//LED_ROFF_GON;
				Robot_MoveOmni( 0,1,0);
			}
		}
		else 
		{
			LED_ROFF_GON;
			Robot_MoveOmni( 0,0,1);
			//������磬
		}
	}
}
