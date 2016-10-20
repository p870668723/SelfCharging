#ifndef MPU_I2C_H
#define MPU_I2C_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"
#include "systick.h"



////0表示写
//#define	I2C_Direction_Transmitter   0
////１表示读
//#define	I2C_Direction_Receiver      1	 


#define true 1
#define false 0
#define bool uint8_t


#define TRUE 0
#define FALSE 1

void IIC_Init(void );
//void I2C_Start(void );
//void I2C_Stop(void );
//void I2C_ACK(void );
//u8 I2C_Wait_ACK(void );
//void I2C_WriteByte(u8 cmd);
//u8 I2C_ReadByte(void );
//void I2C_NoACK(void );
bool I2C_ReadBuffer(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf);
bool I2C_WriteBuffer(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf);
bool I2C_Read(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf);
bool I2C_Write(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf);

//bool i2cWriteBuffer(uint8_t addr_, uint8_t reg_, uint8_t len_, uint8_t *data);
bool i2cWrite(uint8_t addr_, uint8_t reg_, uint8_t data);
bool i2cRead(uint8_t addr_, uint8_t reg_, uint8_t len, uint8_t* buf);
void i2cInit(void);
uint16_t i2cGetErrorCounter(void);
static void i2cUnstick(void);

int8_t i2cwrite(uint8_t addr, uint8_t reg, uint8_t len, uint8_t * data);
int8_t i2cread(uint8_t addr, uint8_t reg, uint8_t len, uint8_t *buf);


#ifdef __cplusplus
}
#endif


#endif
