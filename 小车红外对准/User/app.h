#ifndef _APP__H
#define _APP__H
#include "stm32f10x.h"
void Scan_Metal(void);
void Moto_Ctrl(void);
void uart_choise(void);
extern u8 calibrate_Ldc1000_flag,uart_mode;
extern float Expect_ProximityData,Set_Moto_Speed;
extern u32 Scan_ProximityData_Max,Scan_ProximityData_Min;
#endif
