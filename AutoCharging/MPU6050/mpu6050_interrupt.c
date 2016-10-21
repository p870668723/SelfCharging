#include "main.h"
u8 mpu_6050_data_ok_flag=0;
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
void MPU6050_Interrupt_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,ENABLE);
												
	/* config the NVIC */
	NVIC_Configuration();

	/* EXTI line gpio config*/	
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;       
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	 //
  GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* EXTI line mode config */
  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource7); 
  EXTI_InitStructure.EXTI_Line = EXTI_Line7;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure); 
}
void Set_Exti(u8 mode)
{
  EXTI_InitTypeDef EXTI_InitStructure;
  EXTI_InitStructure.EXTI_Line = EXTI_Line7;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
if(mode==0)
  EXTI_InitStructure.EXTI_LineCmd = DISABLE;
else    
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;

  EXTI_Init(&EXTI_InitStructure); 
}
//void hmc5883_Interrupt_Configuration(void)
//{
//    GPIO_InitTypeDef    gpio;
//    NVIC_InitTypeDef    nvic;
//    EXTI_InitTypeDef    exti;
//// 
////	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
////    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,  ENABLE);   
//// 
////	  gpio.GPIO_Pin = GPIO_Pin_2;
////    gpio.GPIO_Mode = GPIO_Mode_IN;
////    gpio.GPIO_OType = GPIO_OType_PP;
////    gpio.GPIO_PuPd = GPIO_PuPd_UP;
////    gpio.GPIO_Speed = GPIO_Speed_50MHz;
////	  GPIO_Init(GPIOD, &gpio);
////    
////    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOD,GPIO_PinSource2); 
////    
//    exti.EXTI_Line = EXTI_Line2;
//    exti.EXTI_Mode = EXTI_Mode_Interrupt;
//    exti.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿中断
//    exti.EXTI_LineCmd = ENABLE;
//    EXTI_Init(&exti);
//    
//	  nvic.NVIC_IRQChannel = EXTI2_IRQn;
//    nvic.NVIC_IRQChannelPreemptionPriority = 0;
//    nvic.NVIC_IRQChannelSubPriority = 1;
//    nvic.NVIC_IRQChannelCmd = ENABLE;
//    NVIC_Init(&nvic);

//}
//MPU6050 外部中断处理函数
void EXTI9_5_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line7) == SET)
    {
        
        //读取MPU6050数据,为了使云台的控制更平滑，
        //使用MPU6050的陀螺仪输出作为速度环反馈
        //单纯使用电调板返回机械角度值做速度环反馈，会有明显振荡现象
        mpu_6050_data_ok_flag=1;
//        MPU6050_ReadData();                                              
//        MPU6050_Data_Filter();
        EXTI_ClearFlag(EXTI_Line7);          
        
    }
//		 if(EXTI_GetITStatus(EXTI_Line8) == SET)
//    {
//        
////        MPU6050_ReadData();                                              
//        EXTI_ClearFlag(EXTI_Line8);          
//        EXTI_ClearITPendingBit(EXTI_Line8);
//    }
}
//void EXTI2_IRQHandler()
//{
//			 if(EXTI_GetITStatus(EXTI_Line2) == SET)
//    {
//         HMC58X3_mgetValues(mxyz);
////        mxyz[0]=mxyz[0]*cos(MPU6050_Angle.Pitch/57.2168)+
////        mxyz[1]*sin(MPU6050_Angle.Pitch/57.2168)*sin(MPU6050_Angle.Rool/57.2168)
////        -mxyz[2]*sin(MPU6050_Angle.Pitch/57.2168)*cos(MPU6050_Angle.Rool/57.2168);
////        mxyz[1]=mxyz[1]*cos(MPU6050_Angle.Rool/57.2168)+mxyz[2]*sin(MPU6050_Angle.Rool/57.2168);
//        EXTI_ClearFlag(EXTI_Line2);          
//        EXTI_ClearITPendingBit(EXTI_Line2);
//    }
//}