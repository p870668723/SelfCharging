#ifndef SERVO_H
#define SERVO_H

#ifdef __cplusplus
extern "C" {
#endif


#include "stm32f4xx.h"
#include "stm32f4xx_tim.h"


void Servo_Init(void );

void HeadUD_Down(void );
void HeadUD_Middle(void );
void HeadUD_Up(void );
	
void HeadLR_Left(void );
void HeadLR_Middle(void );
void HeadLR_Right(void );
	
	
void RightArmOne_Down(void );
void RightArmOne_Middle(void );
void RightArmOne_Up(void );

void RightArmTwo_Inside(void );
void RightArmTwo_Outside(void );

void RightArmThree_Inside(void );
void RightArmThree_Midside(void );
void RightArmThree_Outsize(void );

void LeftArmOne_Down(void );
void LeftArmOne_Middle(void );
void LeftArmOne_Up(void );

void LeftArmTwo_Inside(void );
void LeftArmTwo_Outside(void );

void LeftArmThree_Inside(void );
void LeftArmThree_Middle(void );
void LeftArmThree_Outside(void );

	
#ifdef __cplusplus
}
#endif

#endif




