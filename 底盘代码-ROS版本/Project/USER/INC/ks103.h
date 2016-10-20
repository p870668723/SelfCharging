#ifndef KS103_H
#define KS103_H

#ifdef __cplusplus
 extern "C" {
#endif


#include "stm32f4xx.h"
	 
#define URM_FORWARD    SlaveAddress3
#define URM_LEFT       SlaveAddress4
#define URM_RIGHT      SlaveAddress2

#define SlaveAddress1   0xE8    //IIC写入时的地址字节数据，+1为读取
#define SlaveAddress2   0xd0
#define SlaveAddress3   0xd2
#define SlaveAddress4   0xd4
#define SlaveAddress5   0xd6
#define SlaveAddress6   0xd8
#define SlaveAddress7   0xda
#define SlaveAddress8   0xdc
#define SlaveAddress9   0xde
#define SlaveAddress10  0xe0
#define SlaveAddress11  0xe2
#define SlaveAddress12  0xe4
#define SlaveAddress13  0xe6
#define SlaveAddress14  0xea
#define SlaveAddress15  0xec
#define SlaveAddress16  0xee
#define SlaveAddress17  0xf8
#define SlaveAddress18  0xfa
#define SlaveAddress19  0xfc
#define SlaveAddress20  0xfe



u8 Single_ReadI2C(u8 SlaveAddress,u8 REG_Address);
void Single_WriteI2C(u8 SlaveAddress,u8 REG_Address,u8 REG_data);
u32 Read_KS10X(u8 SlaveAddress);
void Change_Addr(u8 OldAdddr,u8 NewAddr);
float Read_KS10X_Data(u8 SlaveAddress);
void KS10X_Change_Addr_Init(u8 OldAddr,u8 NewAddr);


#ifdef __cplusplus
}
#endif

#endif

