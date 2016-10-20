#ifndef USART1_H__
#define USART1_H__

//C文件里面的函数 要在C++文件里面引用，就需要加入extern "C"
#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f4xx.h"
#include "stm32f4xx_usart.h"
#include "robotmodel.h"


void USART2_Init(void );
void Robot_Report(RobotModel *robot);

#ifdef __cplusplus
}
#endif


#endif

