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
	
		
		/*轮询控制*/
	while(1) 
	{
		
		if(ChargeFlag==1)
		{
			//开始充电，接管底盘的运动控制
			get_irda_data();
			if(sreal_irda_Position>0)//向右偏
			{
				LED_RON_GOFF;
			}
			else if(sreal_irda_Position<0)//向左偏
			{
				LED_RON_GOFF;
			}
			else//已对准
			{
				LED_ROFF_GON;
			}
		}
		else 
		{
			//结束充电，
		}
	}
}
