#include "main.h"
CanRxMsg RxMessage;
CanTxMsg TxMessage;
extern float g_set_speed;
u16 can_flag;
void CAN_Configuration(void)
{
    u16 std_id =0x200,mask=0xcff;
	CAN_InitTypeDef        can;
	CAN_FilterInitTypeDef  can_filter;
	GPIO_InitTypeDef 	   gpio;
	NVIC_InitTypeDef   	   nvic;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,  ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    
	gpio.GPIO_Pin = GPIO_Pin_11;
	gpio.GPIO_Mode = GPIO_Mode_IPU;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio);	//CAN_RX
	gpio.GPIO_Pin = GPIO_Pin_12;	   
    gpio.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &gpio);    //CAN_TX

    nvic.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
    
    nvic.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;  
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);

	CAN_DeInit(CAN1);

	can.CAN_TTCM = DISABLE;
	can.CAN_ABOM = DISABLE;
	can.CAN_AWUM = DISABLE;
	can.CAN_NART = DISABLE;	  
	can.CAN_RFLM = DISABLE;																
	can.CAN_TXFP = ENABLE;		
 	can.CAN_Mode = CAN_Mode_Normal;
	can.CAN_SJW = CAN_SJW_1tq;
	can.CAN_BS1 = CAN_BS1_5tq;
	can.CAN_BS2 = CAN_BS2_3tq;
	can.CAN_Prescaler = 4;     //CAN BaudRate 36/(1+5+3)/4=1Mbps
	CAN_Init(CAN1, &can);
    
	can_filter.CAN_FilterNumber = 0;
	can_filter.CAN_FilterMode = CAN_FilterMode_IdMask;
	can_filter.CAN_FilterScale = CAN_FilterScale_32bit;
//    can_filter.CAN_FilterIdHigh = 0x0000;
//	can_filter.CAN_FilterIdLow = 0x0000;
//	can_filter.CAN_FilterMaskIdHigh = 0x0000;
//	can_filter.CAN_FilterMaskIdLow =0x0000;
    
	can_filter.CAN_FilterIdHigh = std_id<<5;;
	can_filter.CAN_FilterIdLow = 0|CAN_ID_STD;
	can_filter.CAN_FilterMaskIdHigh = mask<<5;
	can_filter.CAN_FilterMaskIdLow = 0x0000;
	can_filter.CAN_FilterFIFOAssignment = 0;
	can_filter.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&can_filter);

    CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
    CAN_ITConfig(CAN1,CAN_IT_TME,ENABLE);
}
unsigned char can_tx_success_flag = 0;
CAN_MOTO_DATE_STRUCT Can_Moto_Date_struct;
static u8 Mode_Chioce_flage=0;
static s16 Value_H,Value_L,Value;
static s16 Value_HH,Value_HL,Value_LH,Value_LL;
void USB_LP_CAN1_RX0_IRQHandler(void)
{

      CanRxMsg rx_message;    
	  uint32_t Mode_date,ID_date;
	  s16 Speed_Value_H,Speed_Value_L,Speed_Value;
	  
    if (CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET) 
	{
		 CAN_Receive(CAN1, CAN_FIFO0, &rx_message);
        if(rx_message.StdId==0x200&&(Can_Moto_Date_struct.MOTO_MODE==ENTER_SPEED_CURRENT_MODE||Can_Moto_Date_struct.MOTO_MODE==ENTER_PWM_MODE))
        {
            switch(Moto_Drive.Can_id)
            {
                case 1:
                Value_H=rx_message.Data[0];
                Value_L=rx_message.Data[1];break;
                case 2:
                Value_H=rx_message.Data[2];
                Value_L=rx_message.Data[3];break;
                case 3:
                Value_H=rx_message.Data[4];
                Value_L=rx_message.Data[5];break;
                case 4:
                Value_H=rx_message.Data[6];
                Value_L=rx_message.Data[7];break;
            }
            switch(Can_Moto_Date_struct.MOTO_MODE)
            {
            case ENTER_PWM_MODE:
                Value=Value_H<<8|Value_L;
                Moto_Drive.PWM_Value=Value;
                MotorSpeedOut(Moto_Drive.PWM_Value*Moto_Drive.Moto_Dir);
                return;
            case ENTER_SPEED_CURRENT_MODE:
                Value=Value_H<<8|Value_L;
                Moto_Drive.Speed_Value=Value; return;
                case ENTER_CURRENT_MODE:     break;
                case ENTER_LOCATION_MODE:    break;
            case ENTER_POSITION_SPEED_CURRENT_MODE: 
                 switch(Moto_Drive.Can_id)
                {
                    case 1:
                    Value_HH=rx_message.Data[0];
                    Value_HL=rx_message.Data[1];
                    Value_LH=rx_message.Data[2];
                    Value_LL=rx_message.Data[3];
                    Moto_Drive.Expectation_Position=(Value_HH<<24)|(Value_HL<<16)|(Value_LH<<8)|(Value_LL);break;
                    case 2:
                    Value_HH=rx_message.Data[4];
                    Value_HL=rx_message.Data[5];
                    Value_LH=rx_message.Data[6];
                    Value_LL=rx_message.Data[7];
                    Moto_Drive.Expectation_Position=(Value_HH<<24)|(Value_HL<<16)|(Value_LH<<8)|(Value_LL); break;                   
                }break;
            }
            
        }


		 if(rx_message.StdId==0x300)
		 {
					 Can_Moto_Date_struct.MOTO_MODE=rx_message.Data[0];
					 
					 switch(Can_Moto_Date_struct.MOTO_MODE)
				   {
					 case ENTER_PWM_MODE:
							TIM2->CCR4=0;
							TIM2->CCR3=0;
      						TIM_Cmd(TIM4, DISABLE); break;
					 case ENTER_SPEED_CURRENT_MODE:
                            Value_H=rx_message.Data[1];
                            Value_L=rx_message.Data[2];
                            Value=Value_H<<8|Value_L;
                            Moto_Drive.PWM_Value =Value;
                            Value_H=rx_message.Data[3];
                            Value_L=rx_message.Data[4];
                            Value=Value_H<<8|Value_L;
                            Moto_Drive.limit_power=Value/10;
                            PID_Power_Struct.Ouput_deltaUk_Max=Moto_Drive.PWM_Value;
						    PID_Power_Struct.Ouput_deltaUk_Min=-Moto_Drive.PWM_Value;
						    PID_Moto_Drive.Ouput_deltaUk_Max=Moto_Drive.limit_power;
						    PID_Moto_Drive.Ouput_deltaUk_Min=-Moto_Drive.limit_power;
                            Moto_Drive.Speed_Value=0;
      						TIM_Cmd(TIM4, ENABLE);break;
					 case ENTER_CURRENT_MODE:     break;
					 case ENTER_LOCATION_MODE:    break;
                     case ENTER_POSITION_SPEED_CURRENT_MODE: 
                            Value_H=rx_message.Data[1];
                            Value_L=rx_message.Data[2];
                            Value=Value_H<<8|Value_L;
                            Moto_Drive.PWM_Value =Value;
                            Value_H=rx_message.Data[3];
                            Value_L=rx_message.Data[4];
                            Value=Value_H<<8|Value_L;
                            Moto_Drive.limit_power=Value/10;
                            Value_H=rx_message.Data[5];
                            Value_L=rx_message.Data[6];
                            Value=Value_H<<8|Value_L;   
                            Moto_Drive.limit_Speed=Value;
                            PID_Power_Struct.Ouput_deltaUk_Max=Moto_Drive.PWM_Value;
						    PID_Power_Struct.Ouput_deltaUk_Min=-Moto_Drive.PWM_Value;
						    PID_Moto_Drive.Ouput_deltaUk_Max=Moto_Drive.limit_power;
						    PID_Moto_Drive.Ouput_deltaUk_Min=-Moto_Drive.limit_power;
                            PID_Position_Struct.Ouput_deltaUk_Max= Moto_Drive.limit_Speed;
                            PID_Position_Struct.Ouput_deltaUk_Min=-Moto_Drive.limit_Speed;
                            Moto_Drive.Speed_Value=0;
      						TIM_Cmd(TIM4, ENABLE);break;
				   }
					  return;
		}


  }
}

void USB_HP_CAN1_TX_IRQHandler(void)
{
    if (CAN_GetITStatus(CAN1,CAN_IT_TME)!= RESET) 
	{
	   CAN_ClearITPendingBit(CAN1,CAN_IT_TME);
		 can_tx_success_flag = 1;
  }
}
