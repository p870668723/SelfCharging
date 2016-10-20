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
void IIC_Start(void);               //����IIC��ʼ�ź�
void IIC_Stop(void);                //����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);         //IIC����һ���ֽ�
u8 IIC_Read_Byte(unsigned char ack);                 //IIC��ȡһ���ֽ�
u8 IIC_Wait_Ack(void);              //IIC�ȴ�ACK�ź�
void IIC_Ack(void);                 //IIC����ACK�ź�
void IIC_NAck(void);                //IIC������ACK�ź�

#ifdef __cplusplus
}
#endif

#endif

