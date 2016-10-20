/*
 * SlamTec Ctrl Bus Protocol
 * Copyright 2009 - 2015 RoboPeak
 * Copyright 2013 - 2015 Shanghai SlamTec Co., Ltd.
 * http://www.slamtec.com
 * All rights reserved.
 */
/*
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, 
 *    this list of conditions and the following disclaimer in the documentation 
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR 
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "uart.h"


static proc_on_rx_t  _rx_proc = NULL;

/*
*    uart receive callback function
*/
void serial_setrecv_backcall( proc_on_rx_t proc)
{
    _rx_proc = proc;
}

/*
*    uart 0 Initialization��config is :115200,8,n,1
*    must modify this function for another config
*/
void uart_init_115200(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART2,&USART_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
	USART_Cmd(USART2,ENABLE);	
  
}


/*
*    send one byte
*/
void put_byte(_u8 c)
{
	USART_SendData(USART2,c);
	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
}

/*
*    send some bytes
*/
void put_bytes(const _u8 *data,_u16 len)
{
    while(len--)
    {
        put_byte(*data);
        data++;
    }
}


/*	
*    uart receive Interrupt
*/


extern "C" void  USART2_IRQHandler(void) {
	
	if(USART_GetITStatus(USART2,USART_IT_RXNE) == SET)
	{
		_u8 temp;
		
		temp = USART_ReceiveData(USART2);
		if(_rx_proc)
			_rx_proc(temp);
		
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
	}
	else if(USART_GetITStatus(USART2,USART_IT_TC) == SET)
	{
		USART_ClearITPendingBit(USART2,USART_IT_TC);
	}
	else if(USART_GetITStatus(USART2,USART_IT_TXE) == SET)
	{
		USART_ClearITPendingBit(USART2,USART_IT_TXE);
	}
}

//void USART2_IRQHandler(void )
//{
//	if(USART_GetITStatus(USART2,USART_IT_RXNE) == SET)
//	{
//		_u8 temp;
//		
//		temp = USART_ReceiveData(USART2);
//		if(_rx_proc)
//			_rx_proc(temp);
//		
//		USART_ClearITPendingBit(USART2,USART_IT_RXNE);
//	}
//}





