#ifndef DBUS_H
#define DBUS_H
#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_usart.h"
#include "stm32f4xx_dma.h"
	 
#include "bankCommon.h"

//遥控器发送过来的数据结构体


extern CONTROL_MODE Control_Mode;
extern double target_vx,target_vtheta;
extern double last_target_vx,last_target_vtheta;

void DBUS_Init(void );

#ifdef __cplusplus
}
#endif

#endif

