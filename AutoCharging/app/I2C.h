#ifndef _I2C_H
#define _I2C_H

#include "stm32f10x.h"

void IIC_GPIO_Init(void);
int IIC_WriteData(u8 dev_addr,u8 reg_addr,u8 data);
int IIC_ReadData(u8 dev_addr,u8 reg_addr,u8 *pdata,u8 count);
void HEAT_Configuration(void);

#endif
