#ifndef I2C_H
#define I2C_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"

#define SCL_LOW()									GPIO_ResetBits(GPIOH,GPIO_Pin_4)
#define SCL_HIGH()								GPIO_SetBits(GPIOH,GPIO_Pin_4)
#define SDA_LOW()									GPIO_ResetBits(GPIOH,GPIO_Pin_5)
#define SDA_HIGH()								GPIO_SetBits(GPIOH,GPIO_Pin_5)

#define SDA_READ                  GPIO_ReadInputDataBit(GPIOH,GPIO_Pin_5)

 
void I2C2_Init(void);
void IIC_Start(void);               //发送IIC开始信号
void IIC_Stop(void);                //发送IIC停止信号
void IIC_Send_Byte(u8 txd);         //IIC发送一个字节
u8 IIC_Read_Byte(unsigned char ack);                 //IIC读取一个字节
u8 IIC_Wait_Ack(void);              //IIC等待ACK信号
void IIC_Ack(void);                 //IIC发送ACK信号
void IIC_NAck(void);                //IIC不发送ACK信号

#ifdef __cplusplus
}
#endif

#endif

