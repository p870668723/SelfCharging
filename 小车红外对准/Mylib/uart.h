#ifndef __USART_H__
#define __USART_H__
#include "stm32f10x.h"
typedef struct {

	int  data1;
	int  data2;
	int  data3;
	int  data4;
	int  data5;
	int  data6;
} TX_data_Struct;

#define STOP 0x00
#define GO   0x01
#define SCAN 0x02
extern TX_data_Struct  BlueTooth_TX_Data;
extern uint16_t BlueToothkey;
extern uint8_t Go_Mode;
void Serial_Key(uint16_t *BlueToothkey);
void USART1_Configuration(void);
void USART1_SendChar(char b);
void USART1_IRQHandler(void);
void USART1_Send_Date( USART_TypeDef* USARTx, u8 *pWord, u16 Len );
void serial_Tx(TX_data_Struct *data);
u8 serialcom(void);
#endif
