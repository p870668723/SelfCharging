#include "bluetooth.h"
#include "bankCommon.h"
#include "pidControl.h"
#include "dbus.h"
#include "oled.h"


#define DATA_POS							1
#define DATA_NEG							2

u8 bluetooth_buffer[Buffer_Size];
u8 Bluetooth_SendBuffer[100];
int size = 0;


void USART2_Config(void )
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef  NVIC_InitStructure;
	DMA_InitTypeDef   DMA_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
	
	/*********************************GPIO********************************************/
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	/**********************************USART***********************************************/
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART2,&USART_InitStructure);
	
	USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE); 
	
	USART_Cmd(USART2,ENABLE);	
	
	//USART2_RX       DMA1_Stream5_Channel4
	/**********************************DMA接收**********************************************/
	DMA_StructInit(&DMA_InitStructure);
	DMA_InitStructure.DMA_Channel = DMA_Channel_4;    
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&(USART2->DR)); 
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)bluetooth_buffer;   
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;   
	DMA_InitStructure.DMA_BufferSize = Buffer_Size;    
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;   
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;    
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;   
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;   
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;   
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;    
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;    
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;   
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;   
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;   
	DMA_Init(DMA1_Stream5, &DMA_InitStructure); 
	
	DMA_ITConfig(DMA1_Stream5,DMA_IT_TC,ENABLE); 
	
	DMA_Cmd(DMA1_Stream5,ENABLE);
	
	/**************************************NVIC**********************************************/
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
}




void USART2_Init(void )
{
	USART2_Config();
}


/*
把下位机的编码器和陀螺仪数据发送到上位机
上位机用字符串来进行处理，所以下位机发过去的数据不能有0
*/
void Bluetooth_Report(double lspd,double rspd,double angle)
{
	int ilspd,irspd,iangle,index_i;
	unsigned char data[20];
	ilspd = lspd * 10;
	irspd = rspd * 10;
	iangle = angle * 100;
	
	//起始和停止位
	data[0] = 'S';
	data[14] = 'E';
	
	
	data[1] = ilspd;
	data[2] = ilspd >> 8;
	data[3] = ilspd >> 16;
	data[4] = ilspd >> 24;
	
	data[5] = irspd;
	data[6] = irspd >> 8;
	data[7] = irspd >> 16;
	data[8] = irspd >> 24;
	
	data[9] = iangle;
	data[10] = iangle >> 8;
	data[11] = iangle >> 16;
	data[12] = iangle >> 24;
	
	data[13] = XorCode(data+1,12);
	
	//发送15个字节
	for(index_i = 0; index_i< 15;index_i++)
	{
		USART_SendData(USART2,data[index_i]);
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
	}
}






/*向上位机汇报里程计和陀螺仪数据*/
void Robot_Report(RobotModel *robot)
{
	double angle_vel,lspd,rspd;
	/*计算出来左右两轮分别走过的距离*/
	robotReportEncoder(robot->v1,robot->v2,robot->v3);
	angle_vel = robot->angle_vel * 57.295f; //转换为degree
	lspd = robotPose.ldistance;
	rspd = robotPose.rdistance;
	
	/*向上位机发送左右两轮距离和角度信息 单位为mm/s degree*/
	Bluetooth_Report(lspd,rspd,angle_vel);
}

/*
处理来自上位机的命令
*/
void Bluetooth_HandleCmdVel(u8 *bluetooth_buffer )
{
	double spd[3];
	int ivx = 0,ivangle = 0;
	double dvx,dvangle;
	
	//解析命令
	int spos = findCharPosFromBegin(bluetooth_buffer,'S');
	if(spos < 0)
	{
		return ;
	}
	else if(spos + 10 < Buffer_Size && bluetooth_buffer[spos + 10] == 'E')
	{
		 unsigned char tmp = XorCode(bluetooth_buffer+spos+1,8);
		 if(tmp == bluetooth_buffer[spos+9])
		 {
			 ivx = (int)bluetooth_buffer[spos+4] << 24  |  (int)bluetooth_buffer[spos+3] << 16 | (int)bluetooth_buffer[spos+2] << 8 | bluetooth_buffer[spos+1];
			 ivangle = (int)bluetooth_buffer[spos+8] << 24  |  (int)bluetooth_buffer[spos+7] << 16 | (int)bluetooth_buffer[spos+6] << 8 | bluetooth_buffer[spos+5];
			 heartbreak_cnt = 0;  //校验通过，则清零心跳
		 }
	}
	
	dvx = ivx;					//速度mm/s
	dvangle = ivangle;  //角度*10
	dvangle = AngleToRad(dvangle / 10.0);
	
	//只有在SLAM模式的时候，才执行来自上位机的命令
	if(Control_Mode == SLAM_MODE)
  {
		last_target_vtheta = target_vtheta;
		last_target_vx = target_vx;
		
		target_vtheta = dvangle;
		target_vx = dvx;
		//Robot_MoveOmni(target_vx,0,target_vtheta); //执行上位机的命令
	}
}

void Bluetooth_SendString(USART_TypeDef* USARTX,u8 *s)
{
	while(*s)
	{
		USART_SendData(USARTX,*s++);
		while(USART_GetFlagStatus(USARTX,USART_FLAG_TXE)==RESET);
	}
}

void Bluetooth_Printf(USART_TypeDef* USARTX,char *fmt,...)
{
	va_list ap;
	
	va_start(ap,fmt);
	vsprintf((char *)Bluetooth_SendBuffer,fmt,ap);
	va_end(ap);
	
	Bluetooth_SendString(USARTX,Bluetooth_SendBuffer);
}

//DMA1
void DMA1_Stream5_IRQHandler(void )
{
	if(DMA_GetITStatus(DMA1_Stream5,DMA_IT_TCIF5) == SET)
	{
		Bluetooth_HandleCmdVel(bluetooth_buffer);
		DMA_ClearITPendingBit(DMA1_Stream5,DMA_IT_TCIF5);
	}
}

