#include "dbus.h"
#include "robotmodel.h"
#include "pidControl.h"
#include "can1.h"



//DMA接收来自己遥控的命令
volatile u8 sbus_rx_buffer[25];
RC_Ctl_t RC_Ctl;

float vx,vy,vtheta;
static float v1,v2,v3;

double target_vx,target_vtheta;
double last_target_vx,last_target_vtheta;


//DBUS GPIO 初始化
void DBUS_GPIO_Config(void )
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	 
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE); 

	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_USART3); 
	 
	//GPIO_DeInit(GPIOC);
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;   
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; 
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
	GPIO_Init(GPIOC,&GPIO_InitStructure); 
	
}


void Robot_RemoteMove(double vx,double vtheta)
{
	
}


void DBUS_UART_Config(void )
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef 	NVIC_InitStructure;
	DMA_InitTypeDef   DMA_InitStructure;
	
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
	
	USART_DeInit(USART3);
	USART_StructInit(&USART_InitStructure);
	USART_InitStructure.USART_BaudRate = 100*1000;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_Even;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx; 
	USART_Init(USART3, &USART_InitStructure);
	
	USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);
	 
	USART_Cmd(USART3, ENABLE);
	
	//nvic init
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
	
  DMA_DeInit(DMA1_Stream1);
	DMA_StructInit(&DMA_InitStructure);
  DMA_InitStructure.DMA_Channel= DMA_Channel_4;
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(USART3->DR);
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)sbus_rx_buffer;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = 18;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_Mode_Normal;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA1_Stream1,&DMA_InitStructure);
  DMA_ITConfig(DMA1_Stream1,DMA_IT_TC,ENABLE);
  DMA_Cmd(DMA1_Stream1,ENABLE);
		
}

//
void DBUS_Init(void )
{
	DBUS_GPIO_Config();
	DBUS_UART_Config();
}


//
void  DBUS_Decode(void )
{
	//遥杆的四个方向
  RC_Ctl.rc.ch0 = (sbus_rx_buffer[0]| (sbus_rx_buffer[1] << 8)) & 0x07ff; // Channel 0
  RC_Ctl.rc.ch1 = ((sbus_rx_buffer[1] >> 3) | (sbus_rx_buffer[2] << 5)) & 0x07ff; // Channel 1
  RC_Ctl.rc.ch2 = ((sbus_rx_buffer[2] >> 6) | (sbus_rx_buffer[3] << 2) | (sbus_rx_buffer[4] << 10)) & 0x07ff;// Channel 2
  RC_Ctl.rc.ch3 = ((sbus_rx_buffer[4] >> 1) | (sbus_rx_buffer[5] << 7)) & 0x07ff; // Channel 3
  
	//遥控的左右开关
	RC_Ctl.rc.s2 = ((sbus_rx_buffer[5] >> 4)& 0x03); // Switch RIGHT
  RC_Ctl.rc.s1 = ((sbus_rx_buffer[5] >> 6)& 0x03);  //!< Switch  LEFT
  
	//鼠标位置数据
	RC_Ctl.mouse.x = sbus_rx_buffer[6] | ((int16_t)sbus_rx_buffer[7]  << 8); //!< Mouse X axis
  RC_Ctl.mouse.y = sbus_rx_buffer[8] | ((int16_t)sbus_rx_buffer[9]  << 8); //!< Mouse Y axis
  RC_Ctl.mouse.z = sbus_rx_buffer[10] | ((int16_t)sbus_rx_buffer[11] << 8); //!< Mouse Z axis
  
	//鼠标按键
	RC_Ctl.mouse.press_l = sbus_rx_buffer[12];  //!< Mouse Left Is Press ?
  RC_Ctl.mouse.press_r = sbus_rx_buffer[13];  //!< Mouse Right Is Press ?
  
	//键盘按钮
	RC_Ctl.key.v = sbus_rx_buffer[14] | ((uint16_t)sbus_rx_buffer[15]  << 8); //!< KeyBoard value
	
	
	//进入遥控模式
	if(RC_Ctl.rc.s1 == RC_SW_UP)
	{
		Control_Mode = REMOTE_MODE;
		//xx yy
		vx = RC_Ctl.rc.ch1 - RC_CH_VALUE_OFFSET;
		vy = RC_Ctl.rc.ch0 - RC_CH_VALUE_OFFSET;
		
		vtheta = RC_Ctl.rc.ch2 - RC_CH_VALUE_OFFSET;
		
		last_target_vx = target_vx;
		last_target_vtheta = target_vtheta;
		
		target_vx = vx;
		target_vtheta = -vtheta / 660.0;
		
		
		//MotorSetRunVelocity(0,1000,1000);
		//Robot_MoveOmni(vx ,0  ,target_vtheta);
	}
	else
	{
		if(Control_Mode == REMOTE_MODE)
		{
			
			last_target_vx = target_vx;
			last_target_vtheta = target_vtheta;
			
			target_vx = 0;
			target_vtheta = 0;
			//Robot_MoveOmni(0,0,0);
		}
		Control_Mode = SLAM_MODE;
	}
}


void DMA1_Stream1_IRQHandler(void )
{
	if(DMA_GetITStatus(DMA1_Stream1,DMA_IT_TCIF1)== SET)
	{
		DMA_ClearITPendingBit(DMA1_Stream1,DMA_IT_TCIF1);
		DBUS_Decode();
	}
}


