#ifndef USART1_H__
#define USART1_H__

//C�ļ�����ĺ��� Ҫ��C++�ļ��������ã�����Ҫ����extern "C"
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

