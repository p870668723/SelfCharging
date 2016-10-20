#ifndef SYSTICK_H
#define SYSTICK_H

#ifdef __cplusplus
 extern "C" {
#endif


#include "stm32f4xx.h"

#define SYSTICK_CLK  168

//#define delay_ms   systick_ms
//#define delay_us   systick_us

void Systick_Init(void );
void systick_ms(u32 t);
void systick_us(int t);	
void get_ms(unsigned long *timestamp);
	 
	 
#ifdef __cplusplus
}
#endif

#endif

