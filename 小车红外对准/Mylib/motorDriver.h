#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#ifdef __cplusplus
 extern "C" {
#endif

//#include "stm32f4xx.h"
//#include "stm32f4xx_gpio.h"
#include "math.h"

#define Crystal                     (168000000.0)              //晶振频率
#define PWM_Freq										(5000.0)                   //PWM频率
#define PWM_Period                  (Crystal / PWM_Freq)       //周期
#define MAX_SPEED                   3000.0                     //最大转速 r/min


/*
*/
#define MotorOne_Enable()           GPIO_ResetBits(GPIOI,GPIO_Pin_9)
#define MotorOne_Disable()					GPIO_SetBits(GPIOI,GPIO_Pin_9)

#define MotorTwo_Enable()           GPIO_ResetBits(GPIOI,GPIO_Pin_10)
#define MotorTwo_Disable()          GPIO_SetBits(GPIOI,GPIO_Pin_10)

#define MotorThree_Enable()         GPIO_ResetBits(GPIOI,GPIO_Pin_11)
#define MotorThree_Disable()        GPIO_SetBits(GPIOI,GPIO_Pin_11)


#define MotorOne_setPos()      			GPIO_ResetBits(GPIOE,GPIO_Pin_2)
#define MotorOne_setNeg()           GPIO_SetBits(GPIOE,GPIO_Pin_2)

#define MotorTwo_setPos()						GPIO_ResetBits(GPIOE,GPIO_Pin_3)
#define MotorTwo_setNeg()						GPIO_SetBits(GPIOE,GPIO_Pin_3)

#define MotorThree_setPos()					GPIO_ResetBits(GPIOE,GPIO_Pin_4)
#define MotorThree_setNeg() 				GPIO_SetBits(GPIOE,GPIO_Pin_4)


void Motor_setDuty(int mNum,float duty);
void Motor_Release(void );
void Motor_Enable(void );
void Motor_setRun(double duty1,double duty2,double duty3 );
void Motor_setDutys(double *duty);
void Motor_Init(void );

#ifdef __cplusplus
}
#endif

#endif


