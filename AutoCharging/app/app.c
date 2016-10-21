#include "stm32f10x.h"
#include "bsp.h"
#include "app.h"
#include "mymath.h"

s32 uOffset_AD0,uOffset_AD1,uOffset_AD2,uOffset_AD3;
s32 ureal_irda_data0,ureal_irda_data1,ureal_irda_data2,ureal_irda_data3;
s32 sreal_irda_Position;
int ChargeFlag=0;//充电标志。为1时表示开始充电。
	
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
  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
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

/*当无红外照射时，获取传感器的零点偏差，即零位校准*/
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
	/*有红外感应的时候，采样值与零点值的偏差，作为红外感应的强度*/
	ureal_irda_data0=(Get_Adc_Average(ADC_Channel_0,10)-uOffset_AD0)*(4096-uOffset_AD0)/4096;
	ureal_irda_data1=(Get_Adc_Average(ADC_Channel_1,10)-uOffset_AD1)*(4096-uOffset_AD1)/4096;     
  ureal_irda_data2=(Get_Adc_Average(ADC_Channel_2,10)-uOffset_AD2)*(4096-uOffset_AD2)/4096;
	ureal_irda_data3=(Get_Adc_Average(ADC_Channel_3,10)-uOffset_AD3)*(4096-uOffset_AD3)/4096; 	
	
	/*红外感应强度差值越大，说明车越歪，需要控制地盘修正值*/
	sreal_irda_Position_1=500*(ureal_irda_data2-ureal_irda_data1)/(ureal_irda_data2+ureal_irda_data1);
	sreal_irda_Position_2=500*(ureal_irda_data0-ureal_irda_data3)/(ureal_irda_data0+ureal_irda_data3);
	
	sreal_irda_Position=sreal_irda_Position_1+sreal_irda_Position_2;
}




/*设置三个轮子的参数，速度为r/min*/
void MotorSetRunVelocity(short rpm1,short rpm2,short rpm3)
{
	double pwm_fac = 0.9;
	CAN_RoboModule_DRV_PWM_Velocity_Mode(0,1,5000*pwm_fac,rpm1);
	CAN_RoboModule_DRV_PWM_Velocity_Mode(0,2,5000*pwm_fac,rpm2);
	CAN_RoboModule_DRV_PWM_Velocity_Mode(0,3,5000*pwm_fac,rpm3);
}


/****************************************************************************************
                                  PWM速度模式下的数据指令
temp_pwm的取值范围如下：
0 ~ +5000

temp_velocity的取值范围如下：
-32768 ~ +32767
*****************************************************************************************/
void CAN_RoboModule_DRV_PWM_Velocity_Mode(unsigned char Group,unsigned char Number,short Temp_PWM,short Temp_Velocity)
{
    unsigned short can_id = 0x004;
    CanTxMsg tx_message;
    
    tx_message.IDE = CAN_ID_STD;    //标准帧
    tx_message.RTR = CAN_RTR_DATA;  //数据帧
    tx_message.DLC = 0x08;          //帧长度为8
    
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
    
    tx_message.StdId = can_id;      //帧ID为传入参数的CAN_ID

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
    //while(can_tx_success_flag == 0); //如果CAN芯片是TJA1050，注释掉这个判断。
}


