#ifndef __STMFLASH_H__
#define __STMFLASH_H__
#include "stm32f10x.h"
#include "flash.h"
#include "control.h"

void Nvic_Init(void);

void EE_INIT(void);
void EE_SAVE_ACC_OFFSET(void);
void EE_READ_ACC_OFFSET(void);
void EE_SAVE_GYRO_OFFSET(void);
void EE_READ_GYRO_OFFSET(void);
void EE_SAVE_PID(void);
void EE_READ_PID(void);

#endif









