#include "myos.h"
#include <stm32f4xx.h>
#include "stm32f4xx_rcc.h"

OS_STK   TaskStk[OS_MAX_TASKS][OS_TASK_STK_SIZE];
void systick_init(void) //滴答初始化。用来产生时间片  
{  
	RCC_ClocksTypeDef  rcc_clocks;  
	RCC_GetClocksFreq(&rcc_clocks);    
	SysTick_Config(rcc_clocks.HCLK_Frequency / OS_TICKS_PER_SEC);
//	NVIC_SetPriority()
						//OS_TICKS_PER_SEC is defined in os_cfg.h
}
