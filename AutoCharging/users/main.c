#include "stm32f10x.h"
#include "bsp.h"
#include "I2C.h"
#include "app.h"


int main(void)
{
	BSP_Init();
//	if(RxMessage.StdId==0x100)
//	{
//		a=RxMessage.Data[0];
//		
//	}
	
		
		/*��ѯ����*/
	while(1) 
	{
		
		if(ChargeFlag==1)
		{
			//��ʼ��磬�ӹܵ��̵��˶�����
			get_irda_data();
			if(sreal_irda_Position>0)//����ƫ
			{
				LED_RON_GOFF;
			}
			else if(sreal_irda_Position<0)//����ƫ
			{
				LED_RON_GOFF;
			}
			else//�Ѷ�׼
			{
				LED_ROFF_GON;
			}
		}
		else 
		{
			//������磬
		}
	}
}
