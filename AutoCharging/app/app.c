#include "stm32f10x.h"
#include "bsp.h"
#include "app.h"
#include "mymath.h"

s32 uOffset_AD0,uOffset_AD1,uOffset_AD2,uOffset_AD3;
s32 ureal_irda_data0,ureal_irda_data1,ureal_irda_data2,ureal_irda_data3;
s32 sreal_irda_Position;
int ChargeFlag=0;//����־��Ϊ1ʱ��ʾ��ʼ��硣
	
void delay_us(u16 nus)
{
	u16 i=0;
	while(nus--)
	{
		i=11*4;
		while(i--);
	}
} 
void delay_ms(u16 nms)
{
	u16 i=0;
	for(i=nms;i;i--)
	{
		delay_us(1000);
	}
} 


u16 Get_Adc(u8 ch) 
{
  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//�ȴ�ת������

	return ADC_GetConversionValue(ADC1);	//�������һ��ADC1�������ת�����
}

u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_us(5000);
	}
	return temp_val/times;
} 	 

/*���޺�������ʱ����ȡ�����������ƫ�����λУ׼*/
void offset_ad(void)
{
	u32 i;
		for(i=0; i<1000; i++)
	{
	  uOffset_AD0+=Get_Adc_Average(ADC_Channel_0,10);
	  uOffset_AD1+=Get_Adc_Average(ADC_Channel_1,10);
		uOffset_AD2+=Get_Adc_Average(ADC_Channel_2,10);
	  uOffset_AD3+=Get_Adc_Average(ADC_Channel_3,10);
	  delay_us(500);
	}
	uOffset_AD0/=1000;
	uOffset_AD1/=1000;
	uOffset_AD2/=1000;
	uOffset_AD3/=1000;
}


void get_irda_data(void)
{
	s32 sreal_irda_Position_1,sreal_irda_Position_2;
	/*�к����Ӧ��ʱ�򣬲���ֵ�����ֵ��ƫ���Ϊ�����Ӧ��ǿ��*/
	ureal_irda_data0=(Get_Adc_Average(ADC_Channel_0,10)-uOffset_AD0)*(4096-uOffset_AD0)/4096;
	ureal_irda_data1=(Get_Adc_Average(ADC_Channel_1,10)-uOffset_AD1)*(4096-uOffset_AD1)/4096;     
  ureal_irda_data2=(Get_Adc_Average(ADC_Channel_2,10)-uOffset_AD2)*(4096-uOffset_AD2)/4096;
	ureal_irda_data3=(Get_Adc_Average(ADC_Channel_3,10)-uOffset_AD3)*(4096-uOffset_AD3)/4096; 	
	
	/*�����Ӧǿ�Ȳ�ֵԽ��˵����Խ�ᣬ��Ҫ���Ƶ�������ֵ*/
	sreal_irda_Position_1=500*(ureal_irda_data2-ureal_irda_data1)/(ureal_irda_data2+ureal_irda_data1);
	sreal_irda_Position_2=500*(ureal_irda_data0-ureal_irda_data3)/(ureal_irda_data0+ureal_irda_data3);
	
	sreal_irda_Position=sreal_irda_Position_1+sreal_irda_Position_2;
}




/*�����������ӵĲ������ٶ�Ϊr/min*/
void MotorSetRunVelocity(short rpm1,short rpm2,short rpm3)
{
	double pwm_fac = 0.9;
	CAN_RoboModule_DRV_PWM_Velocity_Mode(0,1,5000*pwm_fac,rpm1);
	CAN_RoboModule_DRV_PWM_Velocity_Mode(0,2,5000*pwm_fac,rpm2);
	CAN_RoboModule_DRV_PWM_Velocity_Mode(0,3,5000*pwm_fac,rpm3);
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
    
    //can_tx_success_flag = 0;
    CAN_Transmit(CAN1,&tx_message);
    //while(can_tx_success_flag == 0); //���CANоƬ��TJA1050��ע�͵�����жϡ�
}


