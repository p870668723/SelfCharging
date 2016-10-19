#include <stm32f10x.h>
#include "ldc1000.h"
#include "stdio.h"
#define SPI_RWBIT 		0x80 				                           

#define SPI_RWBIT 		0x80

u32 ProximityData;
u32 FrequencyData; 
float Proximity;
float Frequency;
u32 ProximityData10[10];
u32 FrequencyData10[10];  

__IO uint16_t  DataRcv[5] ;  
 /********************************************************************
 *	@brief:		SPI
 *	@param:		none
 *	@return:	none
 * _____________						  _______________
 * 				|PB13(SSI2CLK)---->	SCLK  |
 * 				|PB12(SSI2FSS)---->	  CSB |
 * 				|PB14(SSI2RX) <----   SDO |
 *  stm32		|PB15(SSI2TX)	---->SDI  |	LDC1000
 * 				|PB0(Timer CLK)----> TBCLK|
 * _____________|						  |______________
 *********************************************************************/
void SPI2_Init(void)
{

 SPI_InitTypeDef  SPI_InitStructure;
 GPIO_InitTypeDef GPIO_InitStructure;
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOB, ENABLE);
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 |GPIO_Pin_14| GPIO_Pin_15;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
 GPIO_Init(GPIOB, &GPIO_InitStructure);

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 GPIO_Init(GPIOB, &GPIO_InitStructure);
	
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 ,ENABLE);
   
 SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
 SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
 SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
 SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
 SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
 SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
 SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
 SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
 SPI_InitStructure.SPI_CRCPolynomial = 7;
 SPI_Init(SPI2, &SPI_InitStructure);
 SPI_Cmd(SPI2, ENABLE);
}
/***********************��LDC�ж�/дһ���ֽ�***********************
*��    ��: ��LDC�ж�/дһ���ֽ�
*��    ��:  д�������
*�� �� ֵ:  ��ȡ�õ�����
*��    ע:
*****************************************************************/
u8 SPI_LDC_RW(u8 data) //��LDC�ж�дһ���ֽ�
{
    /* �� SPI���ͻ������ǿ�ʱ�ȴ� */
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);

    /* ͨ�� SPI2����һ�ֽ����� */
    SPI_I2S_SendData(SPI2, data);
	 
	 /* ��SPI���ջ�����Ϊ��ʱ�ȴ� */
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET);

    /* Return the byte read from the SPI bus */
    return SPI_I2S_ReceiveData(SPI2);
}

/***********************��LDCָ���ļĴ���дֵ***********************
*��    ��: ��LDCָ���ļĴ���дֵ
*��    ��: reg:LDC������+�Ĵ�����ַ��
*    	   data:��Ҫ��Ĵ���д�������
*�� �� ֵ: �Ĵ�����״̬
*��    ע:
*****************************************************************/
static void SPI_LDC_WriteReg(u8 reg,u8 data)
{
//    u8 status;

//    LDC_CE_LOW();//�������ģʽ1���͹���ģʽ����ģʽ��LDC���Խ�������
    LDC_CSN_LOW();//����csnƬѡ�źţ�ʹ��LDC��spi����

    SPI_LDC_RW(reg);//��LDC��������ͼĴ����ĺ�

    SPI_LDC_RW(data);//��ղ�ָ���ļĴ���д������

    LDC_CSN_HIGH();//����csnƬѡ�źţ����ͷ�LDC��spi������ϣ�
}

/***********************��ȡLDCָ���ļĴ���ֵ***********************
*��    ��: ��ȡLDCָ���ļĴ���ֵ
*��    ��: reg:LDC�Ĵ�����ַ��
*�� �� ֵ: ״̬�Ĵ���������
*��    ע:
*****************************************************************/
static u8 SPI_LDC_ReadReg(u8 reg)
{
    u8 Reg_Value;
//    LDC_CE_LOW();//�������ģʽ1���͹���ģʽ����ģʽ��LDC���Խ�������
    LDC_CSN_LOW();//����csnƬѡ�źţ�ʹ��LDC��spi����

    SPI_LDC_RW(reg|SPI_RWBIT);//ѡ��Ĵ���
	  Reg_Value = SPI_LDC_RW(SPI_NOP);

    LDC_CSN_HIGH();//����csnƬѡ�źţ����ͷ�LDC��spi������ϣ�

    return Reg_Value;
}

/********************************************************************
 * @brief��		LDC1000��ʼ������,ps:��SPI������������λ16�����ݳ��ȣ���
 * 				�ڷ�������ʱ���Խ���ַ��ֵ���л�����һ���ͳ�ȥ
 * @param:		none
 * @return:		none
 *********************************************************************/
void LDC1000_init(void)
{
//	SPI_Write(LDC1000_CMD_RPMAX<<8|0xff);
	SPI_LDC_WriteReg(LDC1000_CMD_RPMAX,0x0f);					//����Rp_MAX(0x01)�Ĵ���
	SPI_LDC_WriteReg(LDC1000_CMD_RPMIN,0x39);					//����Rp_MIN(0x02)�Ĵ���
	SPI_LDC_WriteReg(LDC1000_CMD_SENSORFREQ,0x94);						//����Sensor Frequency(0x03)�Ĵ���
	SPI_LDC_WriteReg(LDC1000_CMD_LDCCONFIG,0x17);							//����LDC Configuration(0x04)�Ĵ���
	SPI_LDC_WriteReg(LDC1000_CMD_CLKCONFIG,0x00);							//����Clock Configuration(0x05)�Ĵ���,
																		//ʹ��TBCLK��Ϊʱ��Դ																		//����INTBΪ�Ƚ������־λ(status of Comparator output)
	SPI_LDC_WriteReg(LDC1000_CMD_THRESHILSB,0x50);						//����Comparator Threshold High(0x06)�Ĵ�����8λ
	SPI_LDC_WriteReg(LDC1000_CMD_THRESHIMSB,0x14);						//����Comparator Threshold High(0x07)�Ĵ�����8λ
	SPI_LDC_WriteReg(LDC1000_CMD_THRESLOLSB,0xC0);						//����Comparator Threshold Low(0x08)�Ĵ�����8λ
	SPI_LDC_WriteReg(LDC1000_CMD_INTCONFIG,0x02);							//����INTB Pin Configuration(0x0A),	
	SPI_LDC_WriteReg(LDC1000_CMD_PWRCONFIG,0x01);							//����Power Configuration(0x0B)�Ĵ�����																	//ΪActive Mode,ʹ��ת��
}

/********************************************************************
 * @brief��		ʹ��SPI��ȡLDC1000�е�����
 * @param:		none
 * @return:		none
 *********************************************************************/
void LDCRead(void)
{

	ProximityData = 0;
	FrequencyData = 0;
	DataRcv[0] = SPI_LDC_ReadReg(LDC1000_CMD_PROXLSB);          			//д�뽫Ҫ��ȡ��Proximity Data LSB�Ĵ�����ַ(0x21)  //��ȡ�����Ĵ����е�ֵ��������DataRcv[0]
	DataRcv[1] = SPI_LDC_ReadReg(LDC1000_CMD_PROXMSB);				    	//д�뽫Ҫ��ȡ��Proximity Data MSB�Ĵ�����ַ(0x22)	//��ȡ�����Ĵ����е�ֵ��������DataRcv[1]
	DataRcv[2] = SPI_LDC_ReadReg(LDC1000_CMD_FREQCTRLSB);					//д�뽫Ҫ��ȡ��Frequency Counter Data LSB�Ĵ�����ַ(0x23)//��ȡ�����Ĵ����е�ֵ��������DataRcv[2]
	DataRcv[3] = SPI_LDC_ReadReg(LDC1000_CMD_FREQCTRMID);					//д�뽫Ҫ��ȡ��Frequency Counter Data Mid-Byte�Ĵ�����ַ(0x24)	//��ȡ�����Ĵ����е�ֵ��������DataRcv[3]
	DataRcv[4] = SPI_LDC_ReadReg(LDC1000_CMD_FREQCTRMSB);					//д�뽫Ҫ��ȡ��Frequency Counter Data MSB�Ĵ�����ַ(0x25)	//��ȡ�����Ĵ����е�ֵ��������DataRcv[4]
    ProximityData= DataRcv[0]|(DataRcv[1]<<8) ;//�ϳ�����
	FrequencyData= DataRcv[2]|(DataRcv[3]<<8)|(DataRcv[4]<<16) ;//�ϳ�����
    Proximity=ProximityData;

}

void SPI_LDC_Init(void)
{
	TIM3_Init();
	SPI2_Init();
	LDC1000_init();
}

static void TIM3_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		    // �����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

static void TIM3_Mode_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	u16 CCR3_Val = 1;  

  	/* Time base configuration */		 
  	TIM_TimeBaseStructure.TIM_Period = 2;       //����ʱ����0������999����Ϊ1000�Σ�Ϊһ����ʱ����
  	TIM_TimeBaseStructure.TIM_Prescaler = 0;	    //����Ԥ��Ƶ��Ԥ��Ƶ72����Ϊ1KHz
  	TIM_TimeBaseStructure.TIM_ClockDivision = 0;	//����ʱ�ӷ�Ƶϵ��������Ƶ
  	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //���ϼ���ģʽ

  	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	  TIM_ClearFlag(TIM3, TIM_FLAG_Update);     /* �������жϱ�־ */
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);

	/* PWM1 Mode configuration: Channel3 */
  	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //����ΪPWMģʽ1
  	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	
  	TIM_OCInitStructure.TIM_Pulse = CCR3_Val;	   //��������ֵ�������������������ֵʱ����ƽ��������
  	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  //����ʱ������ֵС��CCR1_ValʱΪ�ߵ�ƽ

  	TIM_OC3Init(TIM3, &TIM_OCInitStructure);	 //ʹ��ͨ��1
  	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);	 //ʹ��CCR3�Ĵ�����Ӱ�ӼĴ�����ֱ�����������¼��ŷ������ģ�

	//TIM_ARRPreloadConfig(TIM3, ENABLE);			 // ʹ��TIM3���ؼĴ���ARR

  	/* TIM3 enable counter */
  	TIM_Cmd(TIM3, ENABLE);                   //ʹ�ܶ�ʱ��3
}


void TIM3_Init(void)
{
	TIM3_GPIO_Config();
	TIM3_Mode_Config();	
}

