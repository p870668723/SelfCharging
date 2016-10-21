#include "bsp.h"

CanTxMsg TxMessage;
CanRxMsg RxMessage;

/********************************************************************************************************
Function Name: BSP_Init
Author       : 
Date         : 2016.7.12
Description  : 
Inputs       : None
Outputs      : None 
********************************************************************************************************/
void BSP_Init(void)
{
	RCC_Configuration();
	GPIO_Configuration();
	NVIC_Configuration();
	CAN1_Configuration();
	ADC_Configuration();
	//IIC_Configuration();
	//DMA_Configuration();
	//PWM_Configuration();
}

/********************************************************************************************************
Function Name: RCC_Configuration
Author       : 
Date         : 2016.7.12
Description  : 
Inputs       : None
Outputs      : None 
********************************************************************************************************/
void RCC_Configuration(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 , ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2 , ENABLE);

}




/********************************************************************************************************
Function Name: GPIO_Configuration
Author       : 
Date         : 2016.7.12
Description  : 
Inputs       : None
Outputs      : None 
********************************************************************************************************/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef gpio;
	gpio.GPIO_Mode=GPIO_Mode_Out_PP;
	gpio.GPIO_Pin=GPIO_Pin_6 | GPIO_Pin_7;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA,&gpio);
	
	GPIO_SetBits(GPIOA,GPIO_Pin_7|GPIO_Pin_6);//初始时，都关闭
}




/********************************************************************************************************
Function Name: NVIC_Configuration
Author       : 
Date         : 2016.7.12
Description  : 
Inputs       : None
Outputs      : None 
********************************************************************************************************/
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef  NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
	
	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	
}


/********************************************************************************************************
Function Name: CAN1_Configuration
Author       : pf
Date         : 2016.7.12
Description  : 
Inputs       : None
Outputs      : None 
********************************************************************************************************/
void CAN1_Configuration(void)
{
	CAN_InitTypeDef        CAN_InitStructure;
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;
	GPIO_InitTypeDef  GPIO_InitStructure;
 
   /* Configure CAN pin: RX */
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
   GPIO_Init(GPIOA, &GPIO_InitStructure);
   
   /* Configure CAN pin: TX */
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_Init(GPIOA, &GPIO_InitStructure);
   
   GPIO_PinRemapConfig(GPIO_Remap1_CAN1 , ENABLE);
   
   /* CAN register init */
   CAN_DeInit(CAN1);
   CAN_StructInit(&CAN_InitStructure);

   /* CAN cell init */
   CAN_InitStructure.CAN_TTCM = DISABLE;
   CAN_InitStructure.CAN_ABOM = DISABLE;
   CAN_InitStructure.CAN_AWUM = DISABLE;
   CAN_InitStructure.CAN_NART = DISABLE;
   CAN_InitStructure.CAN_RFLM = DISABLE;
   CAN_InitStructure.CAN_TXFP = DISABLE;
   CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
   
   /* CAN Baudrate = 1MBps*/
   CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
   CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;
   CAN_InitStructure.CAN_BS2 = CAN_BS2_5tq;
   CAN_InitStructure.CAN_Prescaler = 4;
   CAN_Init(CAN1, &CAN_InitStructure);
 

   CAN_FilterInitStructure.CAN_FilterNumber = 0;
   CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
   CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
   CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
   CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
   CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
   CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
   CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
   CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
   CAN_FilterInit(&CAN_FilterInitStructure);
   
	 CAN_ITConfig(CAN2,CAN_IT_FMP0,ENABLE);
   /* Transmit */
	 
//   TxMessage.StdId = 0x321;
//   TxMessage.ExtId = 0x01;
//   TxMessage.RTR = CAN_RTR_DATA;
//   TxMessage.IDE = CAN_ID_STD;
//   TxMessage.DLC = 1;

}

/********************************************************************************************************
Function Name: RCC_Configuration
Author       : pf
Date         : 2016.7.12
Description  : 
Inputs       : None
Outputs      : None 
********************************************************************************************************/
void IIC_Configuration(void)
{ 

}




/********************************************************************************************************
Function Name: USART_Configuration
Author       : pf
Date         : 2016.7.12
Description  : 
Inputs       : None
Outputs      : None 
********************************************************************************************************/
void USART1_Configuration(void)
{
	USART_InitTypeDef USART_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);  
	
	USART_DeInit(USART1);
	USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	
	USART_Cmd(USART1, ENABLE);
	
}


/********************************************************************************************************
Function Name: RCC_Configuration
Author       : pf
Date         : 2016.7.12
Description  : 
Inputs       : None
Outputs      : None 
********************************************************************************************************/
void DMA_Configuration(void)
{
	DMA_InitTypeDef  DMA_InitStructure;
	
	DMA_DeInit(DMA1_Channel1);
//   DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address;
//   DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC1ConvertedValue;
   DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
   DMA_InitStructure.DMA_BufferSize = 1;
   DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
   DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
   DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
   DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
   DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
   DMA_InitStructure.DMA_Priority = DMA_Priority_High;
   DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
   DMA_Init(DMA1_Channel1, &DMA_InitStructure);  
   /* Enable DMA1 channel1 */
   DMA_Cmd(DMA1_Channel1, ENABLE);
	 DMA_ClearFlag(DMA1_FLAG_TC1);
}


/********************************************************************************************************
Function Name: ADC_Configuration
Author       : 
Date         : 2016.8.30
Description  : 
Inputs       : None
Outputs      : None 
********************************************************************************************************/
void ADC_Configuration(void)
{
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;  //ADC0123通道
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		
	GPIO_Init(GPIOA, &GPIO_InitStructure);	
	
	
	ADC_DeInit(ADC1); 
	
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   

//  ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_7Cycles5);
//	ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 1, ADC_SampleTime_7Cycles5);
	
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
	
	ADC_ResetCalibration(ADC1);	//使能复位校准  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	
	ADC_StartCalibration(ADC1);	 //开启AD校准
 
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
	
}

/********************************************************************************************************
Function Name: PWM_Configuration
Author       : pf
Date         : 2016.7.12
Description  : Configuration of PWM_Configuration of time3
Inputs       : None
Outputs      : None 
********************************************************************************************************/
void PWM_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	int TimerPeriod=0,Channel1Pulse=0;
	
//	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3, ENABLE); 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_11|GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	 TimerPeriod = (SystemCoreClock / 17570 ) - 1;
   /* Compute CCR1 value to generate a duty cycle at 50% for channel 1 and 1N */
   Channel1Pulse = (uint16_t) (((uint32_t) 5 * (TimerPeriod - 1)) / 10);
 
   /* Time Base configuration */
   TIM_TimeBaseStructure.TIM_Prescaler = 0;
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
   TIM_TimeBaseStructure.TIM_Period = TimerPeriod;
   TIM_TimeBaseStructure.TIM_ClockDivision = 0;
 
   TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
 
   /* Channel 1, 2,3 and 4 Configuration in PWM mode */
   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//   TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
   TIM_OCInitStructure.TIM_Pulse = Channel1Pulse;
   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
//   TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
//   TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
//   TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
 
   TIM_OC1Init(TIM3, &TIM_OCInitStructure);

	 TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	 TIM_ARRPreloadConfig(TIM3, ENABLE);
	 
   /* TIM3 counter enable */
   TIM_Cmd(TIM3, ENABLE);
 
   /* TIM3 Main Output Enable */
   TIM_CtrlPWMOutputs(TIM3, ENABLE);

}


/********************************************************************************************************
Function Name: Can_Sent
Author       : pf
Date         : 2016.7.12
Description  : 
Inputs       : data to send
Outputs      : None 
********************************************************************************************************/
void Can_Sent(u8* data)
{
	CanTxMsg tx_message;
	int i=0;
	u8 mbox;
	
	tx_message.StdId = 0x402;
	tx_message.DLC = 0x05;
	tx_message.RTR = CAN_RTR_DATA;
	tx_message.IDE = CAN_ID_STD;

//	for(i=0;i<8;i++)
//	{tx_message.Data[0] = data[i];}
	tx_message.Data[0] = *data;
	tx_message.Data[1] = *(data+1);
	tx_message.Data[2] = *(data+2);
	tx_message.Data[3] = *(data+3);
	tx_message.Data[4] = *(data+4);
	
	mbox=CAN_Transmit(CAN1,&tx_message);
	while(CAN_TransmitStatus(CAN1,mbox)==CAN_TxStatus_Ok); //等待发送成功
}



/********************************************************************************************************
中断服务函数
********************************************************************************************************/
void USB_LP_CAN1_RX0_IRQHandler(void)
{
	CanRxMsg RxMessage;
	u8 data[8]={0};
	u8 i=0;
	if (CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET) 
	{
		CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
		CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
	}
	if(RxMessage.StdId==0x00)//主控板的CAN ID
	{
		for(i=0;i<8;i++)
		{data[i]=RxMessage.Data[i];}
		if(data[0]==1) ChargeFlag=1;//控制板发来充电信号，讲充电标志位置位，主程序开始充电程序。
	}
	else if(RxMessage.StdId==0x01)//电源管理的CAN ID
	{
		for(i=0;i<8;i++)
		{data[i]=RxMessage.Data[i];}
		if(data[0]==1) ChargeFlag=0;//电源管理板发来电量饱和信号，将充电标志清除，结束充电
	}
}
