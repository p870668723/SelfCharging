#ifndef BLUEBOOTH_H
#define BLUEBOOTH_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"
#include "stm32f4xx_usart.h"
	 
#include "stdio.h"
#include "stdint.h"
#include "stdarg.h"
	 
#include "bankCommon.h"
#include "robotmodel.h"

void Bluetooth_Init(void );

void Bluetooth_Printf(USART_TypeDef* USARTX,char *fmt,...);
void Bluetooth_Report(double lspd,double rspd,double angle);
void Robot_Report(RobotModel *robot);

#ifdef __cplusplus
}
#endif

#endif


