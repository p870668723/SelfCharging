#ifndef BOARD_H
#define BOARD_H

#ifdef __cplusplus
 extern "C" {
#endif


#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"

#define Beep_On()			     GPIO_SetBits(GPIOH,GPIO_Pin_3)
#define Beep_Off()         GPIO_ResetBits(GPIOH,GPIO_Pin_3)

#define LEDR_On()          GPIO_SetBits(GPIOE,GPIO_Pin_9)
#define LEDR_Off()         GPIO_ResetBits(GPIOE,GPIO_Pin_9)

#define LEDG_On()          GPIO_SetBits(GPIOE,GPIO_Pin_10)
#define LEDG_Off()         GPIO_ResetBits(GPIOE,GPIO_Pin_10)

#define LEDB_On()          GPIO_SetBits(GPIOE,GPIO_Pin_11)
#define LEDB_Off()         GPIO_ResetBits(GPIOE,GPIO_Pin_11)

#define KEY1_Input()       GPIO_ReadInputDataBits(GPIOE,GPIO_Pin_7)
#define KEY2_Input()       GPIO_ReadInputDataBits(GPIOE,GPIO_Pin_8)


void Board_Init(void );

#ifdef __cplusplus
}
#endif

#endif

