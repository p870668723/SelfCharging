#include "main.h"
#define Uart1_Rx_LEN		128
uint8_t Uart1_Rx[Uart1_Rx_LEN];
uint16_t BlueToothkey=0;
uint8_t Go_Mode;
TX_data_Struct  BlueTooth_TX_Data;
void USART1_Configuration(void)
{
    USART_InitTypeDef usart;
    GPIO_InitTypeDef  gpio;
    NVIC_InitTypeDef  nvic;
    DMA_InitTypeDef   dma;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);

    gpio.GPIO_Pin = GPIO_Pin_10;
    gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA,&gpio);

    gpio.GPIO_Pin = GPIO_Pin_9;  
    gpio.GPIO_Mode = GPIO_Mode_AF_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA,&gpio);

    USART_DeInit(USART1);
    usart.USART_BaudRate = 115200;
    usart.USART_WordLength = USART_WordLength_8b;
    usart.USART_StopBits = USART_StopBits_1;
    usart.USART_Parity = USART_Parity_No ;
    usart.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    usart.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   
    USART_Init(USART1,&usart);
		
		 USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);   
		 USART_ITConfig(USART1, USART_IT_RXNE ,DISABLE);
     USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);
	   USART_Cmd(USART1,ENABLE);
    
    dma.DMA_PeripheralBaseAddr = (uint32_t)&(USART1->DR);
    dma.DMA_MemoryBaseAddr = (uint32_t)Uart1_Rx;   
    dma.DMA_DIR = DMA_DIR_PeripheralSRC;
    dma.DMA_BufferSize = Uart1_Rx_LEN;
    dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
    dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    dma.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;
    dma.DMA_Mode = DMA_Mode_Circular;
    dma.DMA_Priority = DMA_Priority_VeryHigh;
    dma.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel5,&dma);
    DMA_ITConfig(DMA1_Channel5, DMA_IT_TC, ENABLE);
		
	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
    nvic.NVIC_IRQChannel = DMA1_Channel5_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
		
		
   
   nvic.NVIC_IRQChannel = USART1_IRQn;
   nvic.NVIC_IRQChannelPreemptionPriority = 2;
   nvic.NVIC_IRQChannelSubPriority = 1;
   nvic.NVIC_IRQChannelCmd = ENABLE;
   NVIC_Init(&nvic); 
   DMA_Cmd(DMA1_Channel5, ENABLE);	 
   
}

void USART1_SendChar(char b)
{
    while( USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
	USART_SendData(USART1,b);
}

int fputc(int ch, FILE *f)
{
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
    USART_SendData(USART1, (uint8_t)ch);
    return ch;
}

void DMA1_Channel5_IRQHandler() 
{
    if(DMA_GetITStatus(DMA1_IT_TC5) == SET)
    {
        DMA_ClearFlag(DMA1_FLAG_TC5);
        DMA_ClearITPendingBit(DMA1_IT_TC5);
    }
}
void USART1_Send_Date( USART_TypeDef* USARTx, u8 *pWord, u16 Len )
{
	u8 i = 0;
	for(i=0; i<Len; i++) {
		USART_SendData(USARTx, (uint8_t)*pWord);
		while(USART_GetFlagStatus(USARTx,USART_FLAG_TC) == RESET);
		pWord++;
	}
}

void USART1_IRQHandler(void)                               
{
	uint32_t temp = 0,i;
	
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
    {
		TIM_Cmd(TIM4, DISABLE);
    	temp = USART1->SR;
    	temp = USART1->DR;
    	DMA_Cmd(DMA1_Channel5,DISABLE);
	  	temp = Uart1_Rx_LEN - DMA_GetCurrDataCounter(DMA1_Channel5);
        if(uart_mode)
        {
		for(i=0;i<temp;i++)
        ANO_DT_Data_Receive_Prepare(Uart1_Rx[i]);
        }
        else
        serialcom();
		DMA_SetCurrDataCounter(DMA1_Channel5,Uart1_Rx_LEN);
		DMA_Cmd(DMA1_Channel5,ENABLE);
			TIM_Cmd(TIM4, ENABLE);
    } 
	__nop(); 
} 

u8 serialcom(void)
{
	

  if((Uart1_Rx[0] == 0xA5)  && (Uart1_Rx[1] == 0x5A))  //识别帧头
  {
		
		if(Uart1_Rx[4]==((Uart1_Rx[2]+Uart1_Rx[3])&0x00ff))
		{
		}
		else
		{
		Uart1_Rx[0]=0;          //类似于清标志位
	  Uart1_Rx[1]=0;
	  Uart1_Rx[2]=0;
			return 0;
		}
			
    switch(Uart1_Rx[3])			  //串口助手以16进制发送，比如FFAAxx，其中xx用来选择不同的case
	{
	  case 0xb1:                //按键1
	  Uart1_Rx[0]=0;          //类似于清标志位
	  Uart1_Rx[1]=0;
	  Uart1_Rx[2]=0;
      BlueToothkey|=0x01;
	  break;
		
	  case 0xb2:                 //按键2
	  Uart1_Rx[0]=0;          //类似于清标志位
	  Uart1_Rx[1]=0;
	  Uart1_Rx[2]=0;
      BlueToothkey|=0x02;
	  break;
		
		case 0xb3:                  //按键3
	  Uart1_Rx[0]=0;          //类似于清标志位
	  Uart1_Rx[1]=0;
	  Uart1_Rx[2]=0;
        BlueToothkey|=0x04;
	  break;
		
	  case 0xb4:                   //按键4
	  Uart1_Rx[0]=0;          //类似于清标志位
	  Uart1_Rx[1]=0;
	  Uart1_Rx[2]=0;

      BlueToothkey|=0x08;
	  break;
		
		case 0xb5:                    //按键5
	  Uart1_Rx[0]=0;          //类似于清标志位
	  Uart1_Rx[1]=0;
	  Uart1_Rx[2]=0;
      BlueToothkey|=0x10;
	  break;
		
		case 0xb6:                    //按键6
	  Uart1_Rx[0]=0;          //类似于清标志位
	  Uart1_Rx[1]=0;
	  Uart1_Rx[2]=0;
      BlueToothkey|=0x20;
    
	  break;
		case 0xb7:                     //按键7
	  Uart1_Rx[0]=0;          //类似于清标志位
	  Uart1_Rx[1]=0;
	  Uart1_Rx[2]=0;
      BlueToothkey|=0x40;
	  break;
		
		case 0xb8:                      //按键8
	  Uart1_Rx[0]=0;          //类似于清标志位
	  Uart1_Rx[1]=0;
	  Uart1_Rx[2]=0;
      BlueToothkey|=0x80;
	  break;
			
		case 0xb9:                       //按键9
	  Uart1_Rx[0]=0;          //类似于清标志位
	  Uart1_Rx[1]=0;
	  Uart1_Rx[2]=0;
      BlueToothkey|=0x100;  
	  break;
		
		case 0xba:                       //按键10
	  Uart1_Rx[0]=0;          //类似于清标志位
	  Uart1_Rx[1]=0;
	  Uart1_Rx[2]=0;

	  break;
		 
		case 0xbb:                        //按键11
	  Uart1_Rx[0]=0;          //类似于清标志位
	  Uart1_Rx[1]=0;
	  Uart1_Rx[2]=0;
      BlueToothkey|=0x200;
	  break;	
		
		case 0xbc:                        //按键12
	  Uart1_Rx[0]=0;          //类似于清标志位
	  Uart1_Rx[1]=0;
	  Uart1_Rx[2]=0;
      BlueToothkey|=0x400;

	  break;

		case 0xf1:                                          //滑块1
	  Uart1_Rx[0]=0;          //类似于清标志位
	  Uart1_Rx[1]=0;
	  Uart1_Rx[2]=0;

		break;
		
		case 0xf2:                                          //滑块2
	  Uart1_Rx[0]=0;          //类似于清标志位
	  Uart1_Rx[1]=0;
	  Uart1_Rx[2]=0;
	

	  break;

		case 0xf3:                                          //滑块3
	  Uart1_Rx[0]=0;          //类似于清标志位
	  Uart1_Rx[1]=0;
	  Uart1_Rx[2]=0;
		
	
	  break;
    
	}}}

void Serial_Key(uint16_t *BlueToothkey)
{
    if(*BlueToothkey&0x01) //按键1
    {
        Go_Mode=GO;
        Go_Time=0;
        S_Moto_L.Encoder_L=0;
        S_Moto_R.Encoder_L=0;
       *BlueToothkey&=~0x01; 
        TIM_Cmd(TIM7, ENABLE);
    }
     else if(*BlueToothkey&0x02) //按键2
    {
        TIM_Cmd(TIM7, DISABLE);
        Go_Mode=STOP;
       *BlueToothkey&=~0x02;
    }
      else if(*BlueToothkey&0x04) //按键3
    {
        Scan_ProximityData_Max=0;
        Scan_ProximityData_Min=0xffffffff;
        Go_Mode=SCAN;
       *BlueToothkey&=~0x04; 
    }
      else if(*BlueToothkey&0x08) //按键4
    {
        Set_Moto_Speed-=100;
       *BlueToothkey&=~0x08; 
    }
      else if(*BlueToothkey&0x10) //按键5
    {
        Expect_ProximityData+=10;
       *BlueToothkey&=~0x10; 
    }
      else if(*BlueToothkey&0x20) //按键6
    {
        Set_Moto_Speed+=100;
       *BlueToothkey&=~0x20; 
    }
          else if(*BlueToothkey&0x40) //按键7
    {
       *BlueToothkey&=~0x40; 
    }
          else if(*BlueToothkey&0x80) //按键8
    {
      Expect_ProximityData-=10;
       *BlueToothkey&=~0x80; 
    }
              else if(*BlueToothkey&0x100) //按键9
    {
     
       *BlueToothkey&=~0x100; 
    }
    
}
	void serial_Tx(TX_data_Struct *data)
	{
		uint8_t tx[]={0xa5,0x5a,0x06,0xc1,0x01,0x00,0x00,0xaa,00};
		tx[3]=0xc1;
		tx[4]=data->data1>>8;
		tx[5]=data->data1&0x00ff;
		tx[6]=(tx[2]+tx[3]+tx[4]+tx[5])&0x00ff;
		USART1_Send_Date(USART1,tx,8);
		
        tx[3]=0xc2;
		tx[4]=data->data2>>8;
		tx[5]=data->data2&0x00ff;
		tx[6]=(tx[2]+tx[3]+tx[4]+tx[5])&0x00ff;
		USART1_Send_Date(USART1,tx,8);
				
		tx[3]=0xc3;
		tx[4]=data->data3>>8;
		tx[5]=data->data3&0x00ff;
		tx[6]=(tx[2]+tx[3]+tx[4]+tx[5])&0x00ff;
		USART1_Send_Date(USART1,tx,8);
						
		tx[3]=0xc4;				
		tx[4]=data->data4>>8;
		tx[5]=data->data4&0x00ff;
		tx[6]=(tx[2]+tx[3]+tx[4]+tx[5])&0x00ff;
		USART1_Send_Date(USART1,tx,8);
								
		tx[3]=0xc5;							
		tx[4]=data->data5>>8;
		tx[5]=data->data5&0x00ff;
		tx[6]=(tx[2]+tx[3]+tx[4]+tx[5])&0x00ff;
		USART1_Send_Date(USART1,tx,8);
										
		tx[3]=0xc6;								
		tx[4]=data->data6>>8;
		tx[5]=data->data6&0x00ff;
		tx[6]=(tx[2]+tx[3]+tx[4]+tx[5])&0x00ff;
		USART1_Send_Date(USART1,tx,8);
		
	}