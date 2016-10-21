#ifndef __MPU6050_INTERRUPT_H__
#define __MPU6050_INTERRUPT_H__
#include <stm32f10x.h>

void MPU6050_Interrupt_Configuration(void);
void hmc5883_Interrupt_Configuration(void);
void Set_Exti(u8 mode);
//extern float target_pitch_angle;
//extern float target_yaw_angle;
//extern float this_203_angle;
//extern float velocity_203_output;  //yaw轴速度环函数的输出值
//extern float position_203_output;
extern u8 mpu_6050_data_ok_flag;
#endif
