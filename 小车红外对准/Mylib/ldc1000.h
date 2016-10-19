/************************************************************************
�������ܣ�Ұ�𿪷���LDC24L01����ģ���ӳ���
�� �� �ˣ������ʵ��ѧ|�Զ���ѧԺ|2011��|�����
��    ����1.0
���ʱ�䣺2013.7.31
��汾  ��v3.5.0
�޸�ʱ�䣺
************************************************************************/

/******************************************************************************
*
*���˵�����ó���ʵ����STM32��Ƭ��LDC24L01��һϵ�в���
*ע������û���ʹ�øÿ��ʱ������ȵ��ó�ʼ��SPI_LDC_Init������������LDC24L01
*		   �������ã�Ȼ����Ե���LDC_Check�������������Ӽ�⣬��������SUCESS,ʧ��
*			����ERROR,���������Ĳ������û��ɵ���USER_LDC_MODE_TX������USER_LDC_MODE_RX��
*			���ж�д���ã�Ȼ�������LDC_TX_DATA������LDC_RX_DATA�������ͻ��������
*
*�ӿڶ��壺SCK��MISO,MOSI���ţ�GPIOA^5,GPIOA^6,GPIOA^7��������ֲ��
		CSN -- A1 ,CE --A2��IQ--A3��(����������ֲ)
*
******************************************************************************/
#ifndef _LDC24L01_H_
#define _LDC24L01_H_

#include <stm32f10x.h>

/****************������ֲ��*******************/

#define SPI_NOP             0XFF//��ָ��������״̬�Ĵ���

/****************�Ĵ�����ַ��*******************/
#define LDC1000_CMD_REVID 		0x00
#define LDC1000_CMD_RPMAX 		0x01
#define LDC1000_CMD_RPMIN 		0x02
#define LDC1000_CMD_SENSORFREQ 	0x03
#define LDC1000_CMD_LDCCONFIG 	0x04
#define LDC1000_CMD_CLKCONFIG 	0x05
#define LDC1000_CMD_THRESHILSB 	0x06
#define LDC1000_CMD_THRESHIMSB 	0x07
#define LDC1000_CMD_THRESLOLSB 	0x08
#define LDC1000_CMD_THRESLOMSB 	0x09
#define LDC1000_CMD_INTCONFIG 	0x0A
#define LDC1000_CMD_PWRCONFIG 	0x0B
#define LDC1000_CMD_STATUS	 	0x20
#define LDC1000_CMD_PROXLSB 	0x21
#define LDC1000_CMD_PROXMSB 	0x22
#define LDC1000_CMD_FREQCTRLSB	0x23
#define LDC1000_CMD_FREQCTRMID	0x24
#define LDC1000_CMD_FREQCTRMSB	0x25

// LDC BITMASKS
#define LDC1000_BIT_AMPLITUDE    0x18
#define LDC1000_BIT_RESPTIME     0x07
#define LDC1000_BIT_CLKSEL       0x02
#define LDC1000_BIT_CLKPD        0x01
#define LDC1000_BIT_INTMODE      0x07
#define LDC1000_BIT_PWRMODE      0x01
#define LDC1000_BIT_STATUSOSC    0x80
#define LDC1000_BIT_STATUSDRDYB  0x40
#define LDC1000_BIT_STATUSWAKEUP 0x20
#define LDC1000_BIT_STATUSCOMP   0x10

#define TEST_RPMAX_MAX 	0x13			  /**< maximum calibration value for RPMAX */
#define TEST_RPMAX_MIN 	0x10			  /**< minimum calibration value for RPMAX */
#define TEST_RPMAX_INIT TEST_RPMAX_MIN+1  /**< RPMAX initial value */
#define TEST_RPMIN_MAX 	0x3D   			  /**< maximum calibration value for RPMIN */
#define TEST_RPMIN_MIN 	0x3A    		  /**< minimum calibration value for RPMIN */
#define TEST_RPMIN_INIT TEST_RPMIN_MIN+1  /**< RPMIN initial value */
// Final Test Range
#define TEST_RP_MSB_MAX 0x12	/**< maximum value for proximity data */
#define TEST_RP_MSB_MIN 0x0A	/**< minimum value for proximity data */
#define TEST_FC_MAX 0x0D5D		/**< maximum value for frequency counter */
#define TEST_FC_MIN 0x0D39		/**< minimum value for frequency counter */


/****************�궨����*******************/
#define LDC_CSN_HIGH()     GPIO_SetBits(GPIOB, GPIO_Pin_12)
#define LDC_CSN_LOW()      GPIO_ResetBits(GPIOB, GPIO_Pin_12)		 //csn�õ�
#define LDC_Read_IRQ()		GPIO_ReadInputDataBit (GPIOB, GPIO_Pin_1) //�ж�����
extern u32 ProximityData;
extern u32 FrequencyData; 
extern u32 ProximityData10[10];
extern u32 FrequencyData10[10];  
extern float Proximity;
extern float Frequency;
/****************����������*******************/
void SPI2_Init(void);
u8 SPI_LDC_RW(u8 data);
static void SPI_LDC_WriteReg(u8 reg,u8 data);//��reg�Ĵ�����д��data
static u8 SPI_LDC_ReadReg(u8 reg);//��ȡָ��״̬�Ĵ�����ֵ
void LDC1000_init(void);
void LDCRead(void);
void TIM3_NVIC_Configuration(void);
static void TIM3_GPIO_Config(void);
static void TIM3_Mode_Config(void);
void TIM3_Init(void);
void SPI_LDC_Init(void);
#endif
