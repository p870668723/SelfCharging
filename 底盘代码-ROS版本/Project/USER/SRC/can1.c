#include "can1.h"
#include "bankCommon.h"
#include "timer.h"

char   lock = 1;

/*----CAN1_TX-----PA12----*/
/*----CAN1_RX-----PA11----*/

/*************************************************************************
                          CAN1_Configuration
													
��������ʼ��CAN1����Ϊ1M������
*************************************************************************/
void CAN1_Configuration(void)
{
    CAN_InitTypeDef        can;
    CAN_FilterInitTypeDef  can_filter;
    GPIO_InitTypeDef       gpio;
    NVIC_InitTypeDef       nvic;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_CAN1);

    gpio.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_11;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    GPIO_Init(GPIOA, &gpio);
    
    nvic.NVIC_IRQChannel = CAN1_RX0_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 2;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
    
    nvic.NVIC_IRQChannel = CAN1_TX_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);
    
    CAN_DeInit(CAN1);
    CAN_StructInit(&can);
    
    can.CAN_TTCM = DISABLE;
    can.CAN_ABOM = DISABLE;
    can.CAN_AWUM = DISABLE;
    can.CAN_NART = DISABLE;
    can.CAN_RFLM = DISABLE;
    can.CAN_TXFP = ENABLE;
    can.CAN_Mode = CAN_Mode_Normal;
    can.CAN_SJW  = CAN_SJW_1tq;
    can.CAN_BS1 = CAN_BS1_9tq;
    can.CAN_BS2 = CAN_BS2_4tq;
    can.CAN_Prescaler = 3;   //CAN BaudRate 42/(1+9+4)/3=1Mbps
    CAN_Init(CAN1, &can);

		can_filter.CAN_FilterNumber = 0;
		can_filter.CAN_FilterMode = CAN_FilterMode_IdMask;
		can_filter.CAN_FilterScale = CAN_FilterScale_32bit;
		can_filter.CAN_FilterIdHigh = 0x0000;
		can_filter.CAN_FilterIdLow = 0x0000;
		can_filter.CAN_FilterMaskIdHigh = 0x0000;
		can_filter.CAN_FilterMaskIdLow = 0x0000;
		can_filter.CAN_FilterFIFOAssignment = 0;
		can_filter.CAN_FilterActivation=ENABLE;
		CAN_FilterInit(&can_filter);
    
    CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);
    CAN_ITConfig(CAN1,CAN_IT_TME,ENABLE);
}

unsigned char can_tx_success_flag = 0;
/*************************************************************************
                          CAN1_TX_IRQHandler
������CAN1�ķ����жϺ���
*************************************************************************/
void CAN1_TX_IRQHandler(void)
{
    if (CAN_GetITStatus(CAN1,CAN_IT_TME)!= RESET) 
	{
	   CAN_ClearITPendingBit(CAN1,CAN_IT_TME);
       can_tx_success_flag=1;
    }
}

short Real_Current_Value[4] = {0};
short Real_Velocity_Value[4] = {0};
long Real_Position_Value[4] = {0};
char Real_Online[4] = {0};
char Real_Ctl1_Value[4] = {0};
char Real_Ctl2_Value[4] = {0};

//���������ݵĺ�����Ĭ��Ϊ4����������������0�飬���Ϊ1��2��3��4

/*************************************************************************
                          CAN1_RX0_IRQHandler
������CAN1�Ľ����жϺ���
*************************************************************************/
void CAN1_RX0_IRQHandler(void)
{
    CanRxMsg rx_message;
    
    if (CAN_GetITStatus(CAN1,CAN_IT_FMP0)!= RESET)
	  {
        CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
        CAN_Receive(CAN1, CAN_FIFO0, &rx_message);
        
        if((rx_message.IDE == CAN_Id_Standard)&&(rx_message.IDE == CAN_RTR_Data)&&(rx_message.DLC == 8)) //��׼֡������֡�����ݳ���Ϊ8
        {
            if(rx_message.StdId == 0x1B)
            {
							  if(robot_encoder_data.islock == 0)
							  {
									robot_encoder_data.v1 = ((rx_message.Data[4]<<24)|(rx_message.Data[5]<<16)|(rx_message.Data[6]<<8)|(rx_message.Data[7]));
									robot_encoder_data.timestamp[0] = GetSystemTime();
								}
							  
            }
            else if(rx_message.StdId == 0x2B)
            {
							  if(robot_encoder_data.islock == 0)
                {
									robot_encoder_data.v2 = ((rx_message.Data[4]<<24)|(rx_message.Data[5]<<16)|(rx_message.Data[6]<<8)|(rx_message.Data[7]));
									robot_encoder_data.timestamp[1] = GetSystemTime();
								}
						}
            else if(rx_message.StdId == 0x3B)
            {
							  if(robot_encoder_data.islock == 0)
                {
									robot_encoder_data.v3 = ((rx_message.Data[4]<<24)|(rx_message.Data[5]<<16)|(rx_message.Data[6]<<8)|(rx_message.Data[7]));
									robot_encoder_data.timestamp[2] = GetSystemTime();
								}
            }
            else if(rx_message.StdId == 0x4B)
            {
                Real_Current_Value[3] = (rx_message.Data[0]<<8)|(rx_message.Data[1]);
                Real_Velocity_Value[3] = (rx_message.Data[2]<<8)|(rx_message.Data[3]);
                Real_Position_Value[3] = ((rx_message.Data[4]<<24)|(rx_message.Data[5]<<16)|(rx_message.Data[6]<<8)|(rx_message.Data[7]));
            }
            else if(rx_message.StdId == 0x1F)
            {
                Real_Online[0] = 1;
            }
            else if(rx_message.StdId == 0x2F)
            {
                Real_Online[1] = 1;
            }
            else if(rx_message.StdId == 0x3F)
            {
                Real_Online[2] = 1;
            }
            else if(rx_message.StdId == 0x4F)
            {
                Real_Online[3] = 1;
            }
            else if(rx_message.StdId == 0x1C)
            {
                Real_Ctl1_Value[0] = rx_message.Data[0];
                Real_Ctl2_Value[0] = rx_message.Data[1];
            }
            else if(rx_message.StdId == 0x2C)
            {
                Real_Ctl1_Value[1] = rx_message.Data[0];
                Real_Ctl2_Value[1] = rx_message.Data[1];
            }
            else if(rx_message.StdId == 0x3C)
            {
                Real_Ctl1_Value[2] = rx_message.Data[0];
                Real_Ctl2_Value[2] = rx_message.Data[1];
            }
            else if(rx_message.StdId == 0x4C)
            {
                Real_Ctl1_Value[3] = rx_message.Data[0];
                Real_Ctl2_Value[3] = rx_message.Data[1];
            }

        }
                
    }
}

/****************************************************************************************
                                       ��λָ��
*****************************************************************************************/
void CAN_RoboModule_DRV_Reset(unsigned char Group,unsigned char Number)
{
    unsigned short can_id = 0x000;
    CanTxMsg tx_message;
    
    tx_message.IDE = CAN_ID_STD;    //��׼֡
    tx_message.RTR = CAN_RTR_DATA;  //����֡
    tx_message.DLC = 0x08;          //֡����Ϊ8
    
    switch(Group)
    {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7: can_id |= Group<<8; break;
        default: return;
    }
    
    switch(Number)
    {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7:
        case 0x8:
        case 0x9:
        case 0xA:
        case 0xB:
        case 0xC:
        case 0xD:
        case 0xE:
        case 0xF: can_id |= Number<<4; break;
        default: return;
    }
    
    tx_message.StdId = can_id;      //֡IDΪ���������CAN_ID
    
    tx_message.Data[0] = 0x55;
    tx_message.Data[1] = 0x55;
    tx_message.Data[2] = 0x55;
    tx_message.Data[3] = 0x55;
    tx_message.Data[4] = 0x55;
    tx_message.Data[5] = 0x55;
    tx_message.Data[6] = 0x55;
    tx_message.Data[7] = 0x55;
    
    can_tx_success_flag = 0;
    CAN_Transmit(CAN1,&tx_message);
    while(can_tx_success_flag == 0); //���CANоƬ��TJA1050��ע�͵�����жϡ�
}

/****************************************************************************************
                                     ģʽѡ��ָ��
mode��ȡֵ��Χ���£�
PWM_MODE
PWM_CURRENT_MODE
PWM_VELOCITY_MODE
PWM_POSITION_MODE
PWM_VELOCITY_POSITION_MODE
CURRENT_VELOCITY_MODE
CURRENT_POSITION_MODE
CURRENT_VELOCITY_POSITION_MODE
*****************************************************************************************/
void CAN_RoboModule_DRV_Mode_Choice(unsigned char Group,unsigned char Number,unsigned char Mode)
{
    unsigned short can_id = 0x001;
    CanTxMsg tx_message;
    
    tx_message.IDE = CAN_ID_STD;    //��׼֡
    tx_message.RTR = CAN_RTR_DATA;  //����֡
    tx_message.DLC = 0x08;          //֡����Ϊ8
    
    switch(Group)
    {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7: can_id |= Group<<8; break;
        default: return;
    }
    
    switch(Number)
    {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7:
        case 0x8:
        case 0x9:
        case 0xA:
        case 0xB:
        case 0xC:
        case 0xD:
        case 0xE:
        case 0xF: can_id |= Number<<4; break;
        default: return;
    }
    
    tx_message.StdId = can_id;      //֡IDΪ���������CAN_ID
    
    tx_message.Data[0] = Mode;
    tx_message.Data[1] = 0x55;
    tx_message.Data[2] = 0x55;
    tx_message.Data[3] = 0x55;
    tx_message.Data[4] = 0x55;
    tx_message.Data[5] = 0x55;
    tx_message.Data[6] = 0x55;
    tx_message.Data[7] = 0x55;
    
    can_tx_success_flag = 0;
    CAN_Transmit(CAN1,&tx_message);
    while(can_tx_success_flag == 0); //���CANоƬ��TJA1050��ע�͵�����жϡ�
}

/****************************************************************************************
                                  PWMģʽ�µ�����ָ��
temp_pwm��ȡֵ��Χ���£�
-5000 ~ +5000
*****************************************************************************************/
void CAN_RoboModule_DRV_PWM_Mode(unsigned char Group,unsigned char Number,short Temp_PWM)
{
    unsigned short can_id = 0x002;
    CanTxMsg tx_message;
    
    tx_message.IDE = CAN_ID_STD;    //��׼֡
    tx_message.RTR = CAN_RTR_DATA;  //����֡
    tx_message.DLC = 0x08;          //֡����Ϊ8
    
    switch(Group)
    {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7: can_id |= Group<<8; break;
        default: return;
    }
    
    switch(Number)
    {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7:
        case 0x8:
        case 0x9:
        case 0xA:
        case 0xB:
        case 0xC:
        case 0xD:
        case 0xE:
        case 0xF: can_id |= Number<<4; break;
        default: return;
    }
    
    tx_message.StdId = can_id;      //֡IDΪ���������CAN_ID

    if(Temp_PWM > 5000)
    {
        Temp_PWM = 5000;
    }
    else if(Temp_PWM < -5000)
    {
        Temp_PWM = -5000;
    }
    
    tx_message.Data[0] = (unsigned char)((Temp_PWM>>8)&0xff);
    tx_message.Data[1] = (unsigned char)(Temp_PWM&0xff);
    tx_message.Data[2] = 0x55;
    tx_message.Data[3] = 0x55;
    tx_message.Data[4] = 0x55;
    tx_message.Data[5] = 0x55;
    tx_message.Data[6] = 0x55;
    tx_message.Data[7] = 0x55;
    
    can_tx_success_flag = 0;
    CAN_Transmit(CAN1,&tx_message);
    while(can_tx_success_flag == 0); //���CANоƬ��TJA1050��ע�͵�����жϡ�
}

/****************************************************************************************
                                  PWM����ģʽ�µ�����ָ��
temp_pwm��ȡֵ��Χ���£�
0 ~ +5000

temp_current��ȡֵ��Χ���£�
-32768 ~ +32767
*****************************************************************************************/
void CAN_RoboModule_DRV_PWM_Current_Mode(unsigned char Group,unsigned char Number,short Temp_PWM,short Temp_Current)
{
    unsigned short can_id = 0x003;
    CanTxMsg tx_message;
    
    tx_message.IDE = CAN_ID_STD;    //��׼֡
    tx_message.RTR = CAN_RTR_DATA;  //����֡
    tx_message.DLC = 0x08;          //֡����Ϊ8
    
    switch(Group)
    {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7: can_id |= Group<<8; break;
        default: return;
    }
    
    switch(Number)
    {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7:
        case 0x8:
        case 0x9:
        case 0xA:
        case 0xB:
        case 0xC:
        case 0xD:
        case 0xE:
        case 0xF: can_id |= Number<<4; break;
        default: return;
    }
    
    tx_message.StdId = can_id;      //֡IDΪ���������CAN_ID

    if(Temp_PWM > 5000)
    {
        Temp_PWM = 5000;
    }
    else if(Temp_PWM < -5000)
    {
        Temp_PWM = -5000;
    }
    
    if(Temp_PWM < 0)
    {
        Temp_PWM = MyAbs(Temp_PWM);
    }
    
    tx_message.Data[0] = (unsigned char)((Temp_PWM>>8)&0xff);
    tx_message.Data[1] = (unsigned char)(Temp_PWM&0xff);
    tx_message.Data[2] = (unsigned char)((Temp_Current>>8)&0xff);
    tx_message.Data[3] = (unsigned char)(Temp_Current&0xff);
    tx_message.Data[4] = 0x55;
    tx_message.Data[5] = 0x55;
    tx_message.Data[6] = 0x55;
    tx_message.Data[7] = 0x55;
    
    can_tx_success_flag = 0;
    CAN_Transmit(CAN1,&tx_message);
    while(can_tx_success_flag == 0); //���CANоƬ��TJA1050��ע�͵�����жϡ�
}

/****************************************************************************************
                                  PWM�ٶ�ģʽ�µ�����ָ��
temp_pwm��ȡֵ��Χ���£�
0 ~ +5000

temp_velocity��ȡֵ��Χ���£�
-32768 ~ +32767
*****************************************************************************************/
void CAN_RoboModule_DRV_PWM_Velocity_Mode(unsigned char Group,unsigned char Number,short Temp_PWM,short Temp_Velocity)
{
    unsigned short can_id = 0x004;
    CanTxMsg tx_message;
    
    tx_message.IDE = CAN_ID_STD;    //��׼֡
    tx_message.RTR = CAN_RTR_DATA;  //����֡
    tx_message.DLC = 0x08;          //֡����Ϊ8
    
    switch(Group)
    {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7: can_id |= Group<<8; break;
        default: return;
    }
    
    switch(Number)
    {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7:
        case 0x8:
        case 0x9:
        case 0xA:
        case 0xB:
        case 0xC:
        case 0xD:
        case 0xE:
        case 0xF: can_id |= Number<<4; break;
        default: return;
    }
    
    tx_message.StdId = can_id;      //֡IDΪ���������CAN_ID

    if(Temp_PWM > 5000)
    {
        Temp_PWM = 5000;
    }
    else if(Temp_PWM < -5000)
    {
        Temp_PWM = -5000;
    }
    
    if(Temp_PWM < 0)
    {
        Temp_PWM = MyAbs(Temp_PWM);
    }
    
    tx_message.Data[0] = (unsigned char)((Temp_PWM>>8)&0xff);
    tx_message.Data[1] = (unsigned char)(Temp_PWM&0xff);
    tx_message.Data[2] = (unsigned char)((Temp_Velocity>>8)&0xff);
    tx_message.Data[3] = (unsigned char)(Temp_Velocity&0xff);
    tx_message.Data[4] = 0x55;
    tx_message.Data[5] = 0x55;
    tx_message.Data[6] = 0x55;
    tx_message.Data[7] = 0x55;
    
    can_tx_success_flag = 0;
    CAN_Transmit(CAN1,&tx_message);
    while(can_tx_success_flag == 0); //���CANоƬ��TJA1050��ע�͵�����жϡ�
}

/****************************************************************************************
                                  PWMλ��ģʽ�µ�����ָ��
temp_pwm��ȡֵ��Χ���£�
0 ~ +5000

temp_position��ȡֵ��Χ���£�
32λ�з�������
*****************************************************************************************/
void CAN_RoboModule_DRV_PWM_Position_Mode(unsigned char Group,unsigned char Number,short Temp_PWM,long Temp_Position)
{
    unsigned short can_id = 0x005;
    CanTxMsg tx_message;
    
    tx_message.IDE = CAN_ID_STD;    //��׼֡
    tx_message.RTR = CAN_RTR_DATA;  //����֡
    tx_message.DLC = 0x08;          //֡����Ϊ8
    
    switch(Group)
    {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7: can_id |= Group<<8; break;
        default: return;
    }
    
    switch(Number)
    {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7:
        case 0x8:
        case 0x9:
        case 0xA:
        case 0xB:
        case 0xC:
        case 0xD:
        case 0xE:
        case 0xF: can_id |= Number<<4; break;
        default: return;
    }
    
    tx_message.StdId = can_id;      //֡IDΪ���������CAN_ID

    if(Temp_PWM > 5000)
    {
        Temp_PWM = 5000;
    }
    else if(Temp_PWM < -5000)
    {
        Temp_PWM = -5000;
    }
    
    if(Temp_PWM < 0)
    {
        Temp_PWM = MyAbs(Temp_PWM);
    }
    
    tx_message.Data[0] = (unsigned char)((Temp_PWM>>8)&0xff);
    tx_message.Data[1] = (unsigned char)(Temp_PWM&0xff);
    tx_message.Data[2] = 0x55;
    tx_message.Data[3] = 0x55;
    tx_message.Data[4] = (unsigned char)((Temp_Position>>24)&0xff);
    tx_message.Data[5] = (unsigned char)((Temp_Position>>16)&0xff);
    tx_message.Data[6] = (unsigned char)((Temp_Position>>8)&0xff);
    tx_message.Data[7] = (unsigned char)(Temp_Position&0xff);
    
    can_tx_success_flag = 0;
    CAN_Transmit(CAN1,&tx_message);
    while(can_tx_success_flag == 0); //���CANоƬ��TJA1050��ע�͵�����жϡ�
}

/****************************************************************************************
                                  PWM�ٶ�λ��ģʽ�µ�����ָ��
temp_pwm��ȡֵ��Χ���£�
0 ~ +5000

temp_velocity��ȡֵ��Χ���£�
0 ~ +32767

temp_position��ȡֵ��Χ���£�
32λ�з�������
*****************************************************************************************/
void CAN_RoboModule_DRV_PWM_Velocity_Position_Mode(unsigned char Group,unsigned char Number,short Temp_PWM,short Temp_Velocity,long Temp_Position)
{
    unsigned short can_id = 0x006;
    CanTxMsg tx_message;
    
    tx_message.IDE = CAN_ID_STD;    //��׼֡
    tx_message.RTR = CAN_RTR_DATA;  //����֡
    tx_message.DLC = 0x08;          //֡����Ϊ8
    
    switch(Group)
    {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7: can_id |= Group<<8; break;
        default: return;
    }
    
    switch(Number)
    {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7:
        case 0x8:
        case 0x9:
        case 0xA:
        case 0xB:
        case 0xC:
        case 0xD:
        case 0xE:
        case 0xF: can_id |= Number<<4; break;
        default: return;
    }
    
    tx_message.StdId = can_id;      //֡IDΪ���������CAN_ID

    if(Temp_PWM > 5000)
    {
        Temp_PWM = 5000;
    }
    else if(Temp_PWM < -5000)
    {
        Temp_PWM = -5000;
    }
    
    if(Temp_PWM < 0)
    {
        Temp_PWM = MyAbs(Temp_PWM);
    }
    
    if(Temp_Velocity < 0)
    {
        Temp_Velocity = MyAbs(Temp_Velocity);
    }
    
    tx_message.Data[0] = (unsigned char)((Temp_PWM>>8)&0xff);
    tx_message.Data[1] = (unsigned char)(Temp_PWM&0xff);
    tx_message.Data[2] = (unsigned char)((Temp_Velocity>>8)&0xff);
    tx_message.Data[3] = (unsigned char)(Temp_Velocity&0xff);
    tx_message.Data[4] = (unsigned char)((Temp_Position>>24)&0xff);
    tx_message.Data[5] = (unsigned char)((Temp_Position>>16)&0xff);
    tx_message.Data[6] = (unsigned char)((Temp_Position>>8)&0xff);
    tx_message.Data[7] = (unsigned char)(Temp_Position&0xff);
    
    can_tx_success_flag = 0;
    CAN_Transmit(CAN1,&tx_message);
    while(can_tx_success_flag == 0); //���CANоƬ��TJA1050��ע�͵�����жϡ�
}

/****************************************************************************************
                                  �����ٶ�ģʽ�µ�����ָ��
temp_current��ȡֵ��Χ���£�
0 ~ +32767

temp_velocity��ȡֵ��Χ���£�
-32768 ~ +32767
*****************************************************************************************/
void CAN_RoboModule_DRV_Current_Velocity_Mode(unsigned char Group,unsigned char Number,short Temp_Current,short Temp_Velocity)
{
    unsigned short can_id = 0x007;
    CanTxMsg tx_message;
    
    tx_message.IDE = CAN_ID_STD;    //��׼֡
    tx_message.RTR = CAN_RTR_DATA;  //����֡
    tx_message.DLC = 0x08;          //֡����Ϊ8
    
    switch(Group)
    {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7: can_id |= Group<<8; break;
        default: return;
    }
    
    switch(Number)
    {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7:
        case 0x8:
        case 0x9:
        case 0xA:
        case 0xB:
        case 0xC:
        case 0xD:
        case 0xE:
        case 0xF: can_id |= Number<<4; break;
        default: return;
    }
    
    tx_message.StdId = can_id;      //֡IDΪ���������CAN_ID
    
    if(Temp_Current < 0)
    {
        Temp_Current = MyAbs(Temp_Current);
    }
    
    tx_message.Data[0] = (unsigned char)((Temp_Current>>8)&0xff);
    tx_message.Data[1] = (unsigned char)(Temp_Current&0xff);
    tx_message.Data[2] = (unsigned char)((Temp_Velocity>>8)&0xff);
    tx_message.Data[3] = (unsigned char)(Temp_Velocity&0xff);
    tx_message.Data[4] = 0x55;
    tx_message.Data[5] = 0x55;
    tx_message.Data[6] = 0x55;
    tx_message.Data[7] = 0x55;
    
    can_tx_success_flag = 0;
    CAN_Transmit(CAN1,&tx_message);
    while(can_tx_success_flag == 0); //���CANоƬ��TJA1050��ע�͵�����жϡ�
}

/****************************************************************************************
                                  ����λ��ģʽ�µ�����ָ��
temp_current��ȡֵ��Χ���£�
0 ~ +32767

temp_position��ȡֵ��Χ���£�
32λ�з�������
*****************************************************************************************/
void CAN_RoboModule_DRV_Current_Position_Mode(unsigned char Group,unsigned char Number,short Temp_Current,long Temp_Position)
{
    unsigned short can_id = 0x008;
    CanTxMsg tx_message;
    
    tx_message.IDE = CAN_ID_STD;    //��׼֡
    tx_message.RTR = CAN_RTR_DATA;  //����֡
    tx_message.DLC = 0x08;          //֡����Ϊ8
    
    switch(Group)
    {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7: can_id |= Group<<8; break;
        default: return;
    }
    
    switch(Number)
    {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7:
        case 0x8:
        case 0x9:
        case 0xA:
        case 0xB:
        case 0xC:
        case 0xD:
        case 0xE:
        case 0xF: can_id |= Number<<4; break;
        default: return;
    }
    
    tx_message.StdId = can_id;      //֡IDΪ���������CAN_ID
    
    if(Temp_Current < 0)
    {
        Temp_Current = MyAbs(Temp_Current);
    }

    tx_message.Data[0] = (unsigned char)((Temp_Current>>8)&0xff);
    tx_message.Data[1] = (unsigned char)(Temp_Current&0xff);
    tx_message.Data[2] = 0x55;
    tx_message.Data[3] = 0x55;
    tx_message.Data[4] = (unsigned char)((Temp_Position>>24)&0xff);
    tx_message.Data[5] = (unsigned char)((Temp_Position>>16)&0xff);
    tx_message.Data[6] = (unsigned char)((Temp_Position>>8)&0xff);
    tx_message.Data[7] = (unsigned char)(Temp_Position&0xff);
    
    can_tx_success_flag = 0;
    CAN_Transmit(CAN1,&tx_message);
    while(can_tx_success_flag == 0); //���CANоƬ��TJA1050��ע�͵�����жϡ�
}

/****************************************************************************************
                                  �����ٶ�λ��ģʽ�µ�����ָ��
temp_current��ȡֵ��Χ���£�
0 ~ +32767

temp_velocity��ȡֵ��Χ���£�
0 ~ +32767

temp_position��ȡֵ��Χ���£�
32λ�з�������
*****************************************************************************************/
void CAN_RoboModule_DRV_Current_Velocity_Position_Mode(unsigned char Group,unsigned char Number,short Temp_Current,short Temp_Velocity,long Temp_Position)
{
    unsigned short can_id = 0x009;
    CanTxMsg tx_message;
    
    tx_message.IDE = CAN_ID_STD;    //��׼֡
    tx_message.RTR = CAN_RTR_DATA;  //����֡
    tx_message.DLC = 0x08;          //֡����Ϊ8
    
    switch(Group)
    {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7: can_id |= Group<<8; break;
        default: return;
    }
    
    switch(Number)
    {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7:
        case 0x8:
        case 0x9:
        case 0xA:
        case 0xB:
        case 0xC:
        case 0xD:
        case 0xE:
        case 0xF: can_id |= Number<<4; break;
        default: return;
    }
    
    tx_message.StdId = can_id;      //֡IDΪ���������CAN_ID
    
    if(Temp_Current < 0)
    {
        Temp_Current = MyAbs(Temp_Current);
    }

    tx_message.Data[0] = (unsigned char)((Temp_Current>>8)&0xff);
    tx_message.Data[1] = (unsigned char)(Temp_Current&0xff);
    tx_message.Data[2] = (unsigned char)((Temp_Velocity>>8)&0xff);
    tx_message.Data[3] = (unsigned char)(Temp_Velocity&0xff);
    tx_message.Data[4] = (unsigned char)((Temp_Position>>24)&0xff);
    tx_message.Data[5] = (unsigned char)((Temp_Position>>16)&0xff);
    tx_message.Data[6] = (unsigned char)((Temp_Position>>8)&0xff);
    tx_message.Data[7] = (unsigned char)(Temp_Position&0xff);
    
    can_tx_success_flag = 0;
    CAN_Transmit(CAN1,&tx_message);
    while(can_tx_success_flag == 0); //���CANоƬ��TJA1050��ע�͵�����жϡ�
}

/****************************************************************************************
                                      ����ָ��
Temp_Time��ȡֵ��Χ: 0 ~ 255��Ϊ0ʱ��Ϊ�رյ����ٶ�λ�÷�������
Ctl1_Ctl2��ȡֵ��Χ��0 or 1 ������Ϊ0 or 1������Ϊ��0��Ϊ�ر�������λ��⹦��
�ر���ʾ��Ctl1��Ctl2�Ĺ��ܽ�������RMDS-102������汾��������Ctl1_Ctl2 = 0 ����
*****************************************************************************************/
void CAN_RoboModule_DRV_Config(unsigned char Group,unsigned char Number,unsigned char Temp_Time,unsigned char Ctl1_Ctl2)
{
    unsigned short can_id = 0x00A;
    CanTxMsg tx_message;
    
    tx_message.IDE = CAN_ID_STD;    //��׼֡
    tx_message.RTR = CAN_RTR_DATA;  //����֡
    tx_message.DLC = 0x08;          //֡����Ϊ8
    
    switch(Group)
    {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7: can_id |= Group<<8; break;
        default: return;
    }
    
    switch(Number)
    {
        case 0x1: //��ָ�֧�ֹ㲥�����Դ˴�û��0��������
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7:
        case 0x8:
        case 0x9:
        case 0xA:
        case 0xB:
        case 0xC:
        case 0xD:
        case 0xE:
        case 0xF: can_id |= Number<<4; break;
        default: return;
    }
    
    if((Ctl1_Ctl2 != 0x00)&&(Ctl1_Ctl2 != 0x01))
    {
        Ctl1_Ctl2 = 0x00;
    }
    
    tx_message.StdId = can_id;      //֡IDΪ���������CAN_ID
    
    tx_message.Data[0] = Temp_Time;
    tx_message.Data[1] = Ctl1_Ctl2;
    tx_message.Data[2] = 0x55;
    tx_message.Data[3] = 0x55;
    tx_message.Data[4] = 0x55;
    tx_message.Data[5] = 0x55;
    tx_message.Data[6] = 0x55;
    tx_message.Data[7] = 0x55;
    
    can_tx_success_flag = 0;
    CAN_Transmit(CAN1,&tx_message);
    while(can_tx_success_flag == 0); //���CANоƬ��TJA1050��ע�͵�����жϡ�
}

/****************************************************************************************
                                      ���߼��
*****************************************************************************************/
void CAN_RoboModule_DRV_Online_Check(unsigned char Group,unsigned char Number)
{
    unsigned short can_id = 0x00F;
    CanTxMsg tx_message;
    
    tx_message.IDE = CAN_ID_STD;    //��׼֡
    tx_message.RTR = CAN_RTR_DATA;  //����֡
    tx_message.DLC = 0x08;          //֡����Ϊ8
    
    switch(Group)
    {
        case 0x0:
        case 0x1:
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7: can_id |= Group<<8; break;
        default: return;
    }
    
    switch(Number)
    {
        case 0x1: //��ָ�֧�ֹ㲥�����Դ˴�û��0��������
        case 0x2:
        case 0x3:
        case 0x4:
        case 0x5:
        case 0x6:
        case 0x7:
        case 0x8:
        case 0x9:
        case 0xA:
        case 0xB:
        case 0xC:
        case 0xD:
        case 0xE:
        case 0xF: can_id |= Number<<4; break;
        default: return;
    }
    
    tx_message.StdId = can_id;      //֡IDΪ���������CAN_ID
    
    tx_message.Data[0] = 0x55;
    tx_message.Data[1] = 0x55;
    tx_message.Data[2] = 0x55;
    tx_message.Data[3] = 0x55;
    tx_message.Data[4] = 0x55;
    tx_message.Data[5] = 0x55;
    tx_message.Data[6] = 0x55;
    tx_message.Data[7] = 0x55;
    
    can_tx_success_flag = 0;
    CAN_Transmit(CAN1,&tx_message);
    while(can_tx_success_flag == 0); //���CANоƬ��TJA1050��ע�͵�����жϡ�
}





/*�����������ӵĲ������ٶ�Ϊr/min*/
void MotorSetRunVelocity(short rpm1,short rpm2,short rpm3)
{
	double pwm_fac = 0.9;
	CAN_RoboModule_DRV_PWM_Velocity_Mode(0,1,5000*pwm_fac,rpm1);
	CAN_RoboModule_DRV_PWM_Velocity_Mode(0,2,5000*pwm_fac,rpm2);
	CAN_RoboModule_DRV_PWM_Velocity_Mode(0,3,5000*pwm_fac,rpm3);
}
