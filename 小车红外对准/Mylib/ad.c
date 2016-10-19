#include "main.h"
s32 uOffset_AD1,uOffset_AD2;
s32 ureal_irda_data1,ureal_irda_data2;
s32 sreal_irda_Position;
#define ADC1_DR_Address    ((uint32_t)0x4001244C)
#define ADC3_DR_Address    ((uint32_t)0x40013C4C)
u16 ADC1_ConvertedValue[32];
u16 ADC3_ConvertedValue[32];
void ADC1_Configuration(void)
{
    GPIO_InitTypeDef         gpio;
    ADC_InitTypeDef          adc;
    DMA_InitTypeDef          dma;
//    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOC, ENABLE); 
  
    gpio.GPIO_Pin = GPIO_Pin_0;
    gpio.GPIO_Mode = GPIO_Mode_AIN;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC,&gpio);
    
    DMA_DeInit(DMA1_Channel1);
    dma.DMA_PeripheralBaseAddr = ADC1_DR_Address;
    dma.DMA_MemoryBaseAddr = (u32)&ADC1_ConvertedValue;
    dma.DMA_DIR = DMA_DIR_PeripheralSRC;
    dma.DMA_BufferSize = 32;
    dma.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    dma.DMA_MemoryInc = DMA_MemoryInc_Enable;
    dma.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    dma.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    dma.DMA_Mode = DMA_Mode_Circular;
    dma.DMA_Priority = DMA_Priority_High;
    dma.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel1, &dma);
    DMA_Cmd(DMA1_Channel1, ENABLE);
   
    
//    NVIC_InitStructure.NVIC_IRQChannel=DMA1_Channel1_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority=1;
//	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
    
    adc.ADC_Mode = ADC_Mode_Independent;	             
    adc.ADC_ScanConvMode = DISABLE;			             
    adc.ADC_ContinuousConvMode = ENABLE;	               
    adc.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;  
    adc.ADC_DataAlign = ADC_DataAlign_Right;
    adc.ADC_NbrOfChannel = 1;
    ADC_Init(ADC1, &adc);
    
    RCC_ADCCLKConfig(RCC_PCLK2_Div8);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 1, ADC_SampleTime_55Cycles5);
    ADC_DMACmd(ADC1, ENABLE);

    ADC_Cmd(ADC1, ENABLE);
  	ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while(ADC_GetCalibrationStatus(ADC1));
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}
void ADC3_Configuration(void)
{
    GPIO_InitTypeDef         gpio;
    ADC_InitTypeDef          adc;
    DMA_InitTypeDef          DMA_InitStructure;
//    NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3 | RCC_APB2Periph_GPIOC, ENABLE); 
  
    gpio.GPIO_Pin = GPIO_Pin_1;
    gpio.GPIO_Mode = GPIO_Mode_AIN;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC,&gpio);

 /* DMA2 channel5 configuration ----------------------------------------------*/
  DMA_DeInit(DMA2_Channel5);
  DMA_InitStructure.DMA_PeripheralBaseAddr = ADC3_DR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC3_ConvertedValue;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = 32;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(DMA2_Channel5, &DMA_InitStructure);  
  /* Enable DMA2 channel5 */
  DMA_Cmd(DMA2_Channel5, ENABLE);

    adc.ADC_Mode = ADC_Mode_Independent;
    adc.ADC_ScanConvMode = DISABLE;
    adc.ADC_ContinuousConvMode = ENABLE;
    adc.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    adc.ADC_DataAlign = ADC_DataAlign_Right;
    adc.ADC_NbrOfChannel = 1;
    ADC_Init(ADC3, &adc);

    RCC_ADCCLKConfig(RCC_PCLK2_Div8);
    ADC_RegularChannelConfig(ADC3, ADC_Channel_11, 1, ADC_SampleTime_239Cycles5);
    ADC_DMACmd(ADC3, ENABLE);

    ADC_Cmd(ADC3, ENABLE);
  	ADC_ResetCalibration(ADC3);
    while(ADC_GetResetCalibrationStatus(ADC3));
    ADC_StartCalibration(ADC3);
    while(ADC_GetCalibrationStatus(ADC3));
    ADC_SoftwareStartConvCmd(ADC3, ENABLE);
}
int ADC_Filter(void)
{
    int sum = 0;
    int adc_i = 0;

    for(adc_i=0;adc_i<32;adc_i++)
    { 
        sum += ADC1_ConvertedValue[adc_i];
    }
    sum >>=5;
    return sum;
}
#define N 32

uint32_t filter()
{
uint32_t value_buf[N],temp;
char count,i,j;
for ( count=0;count<N;count++)
{
value_buf[count] = ADC3_ConvertedValue[count];
}
for (j=0;j<N-1;j++)
{
for (i=0;i<N-j;i++)
{
   if ( value_buf[i]>value_buf[i+1] )
   {
      temp = value_buf[i];
      value_buf[i] = value_buf[i+1]; 
      value_buf[i+1] = temp;
   }
}
}
return value_buf[(N-1)/2];
} 

int ADC3_Filter(void)
{
    int sum = 0;
    int adc_i = 0;
    
    for(adc_i=0;adc_i<32;adc_i++)
    { 
        sum += ADC3_ConvertedValue[adc_i];
    }
    sum >>=5;
    return sum;
//    return filter();
    
}
void offset_ad()
{
	u32 i;
		for(i=0; i<1000; i++)
	{
	uOffset_AD1+=ADC_Filter();
	uOffset_AD2+=ADC3_Filter();
	delay_ms(1);
	}
	uOffset_AD1/=1000;
	uOffset_AD2/=1000;
}
void get_irda_data()
{
	ureal_irda_data1=(ADC_Filter()-uOffset_AD1)*(4096-uOffset_AD1)/4096;
	ureal_irda_data2=(ADC3_Filter()-uOffset_AD2)*(4096-uOffset_AD2)/4096;                
	sreal_irda_Position=500*(ureal_irda_data1-ureal_irda_data2)/(ureal_irda_data1+ureal_irda_data2);
}