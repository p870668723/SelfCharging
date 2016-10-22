#include "systick.h"


#define SYS_FREQ    72000000

void Systick_Init()
{
	
	SysTick->CTRL  = 0;
}


/*t:0~750*/
void Systick_ms(u32 t)
{
	
		u32 temp=0;
	
	  SysTick->LOAD=(SYS_FREQ/8)/1000*t;
		SysTick->VAL=0;
	  SysTick->CTRL |=SysTick_CTRL_ENABLE_Msk;
	
		do
		{
			temp=SysTick->CTRL;
		}while(!(temp&(1<<16)));
		
	  SysTick->CTRL &=~SysTick_CTRL_ENABLE_Msk;	
}

/*msÑÓÊ±£¬*/
void systick_ms(u32 t)	
{
		while(t>500)
		{
			t-=500;
			Systick_ms(500);
		}
		Systick_ms(t);
}

/*usÑÓÊ±*/
void systick_us(int t)	
{
	  u32 temp=0;
	
	  SysTick->LOAD=(SYS_FREQ/8)/(1000*1000)*t;
		SysTick->VAL=0;
	  SysTick->CTRL |=SysTick_CTRL_ENABLE_Msk;
	
		do
		{
			temp=SysTick->CTRL;
		}while(!(temp&(1<<16)));
		
	  SysTick->CTRL &=~SysTick_CTRL_ENABLE_Msk;	
	
}


void  get_ms(unsigned long *timestamp)
{
	return ;
}
     



