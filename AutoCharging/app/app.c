#include "stm32f10x.h"
#include "bsp.h"
#include "app.h"

s32 uOffset_AD0,uOffset_AD1,uOffset_AD2,uOffset_AD3;
s32 ureal_irda_data0,ureal_irda_data1,ureal_irda_data2,ureal_irda_data3;
s32 sreal_irda_Position;

void delay_us(u16 nus)
{
	u16 i=0;
	while(nus--)
	{
		i=11*4;
		while(i--);
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
void offset_ad()
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


void get_irda_data()
{
	/*有红外感应的时候，采样值与零点值的偏差，作为红外感应的强度*/
	ureal_irda_data0=(Get_Adc_Average(ADC_Channel_0,10)-uOffset_AD0)*(4096-uOffset_AD0)/4096;
	ureal_irda_data1=(Get_Adc_Average(ADC_Channel_1,10)-uOffset_AD1)*(4096-uOffset_AD1)/4096;     
  ureal_irda_data2=(Get_Adc_Average(ADC_Channel_2,10)-uOffset_AD2)*(4096-uOffset_AD2)/4096;
	ureal_irda_data3=(Get_Adc_Average(ADC_Channel_3,10)-uOffset_AD3)*(4096-uOffset_AD3)/4096; 	
	
	/*红外感应强度差值越大，说明车越歪，需要控制地盘修正值*/
	//sreal_irda_Position=500*(ureal_irda_data1-ureal_irda_data2)/(ureal_irda_data1+ureal_irda_data2);
}
