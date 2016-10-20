#include "ks103.h"
#include "I2C.h"
#include "math.h"
#include "systick.h"

#define delay_us               systick_us
#define delay_ms               systick_ms
 
#define uchar unsigned char
#define uint unsigned int
 
u16 sum;    
 
/***************************************************************************
*******************************************************************************/
 
void Single_WriteI2C(u8 SlaveAddress,u8 REG_Address,u8 REG_data)
{       
	IIC_Start();                    //��ʼ�ź�
	
	IIC_Send_Byte(SlaveAddress);   //�����豸��ַ+д�ź�
	IIC_Wait_Ack();
 
	IIC_Send_Byte(REG_Address);    //�ڲ��Ĵ�����ַ
	IIC_Wait_Ack();              

	IIC_Send_Byte(REG_data);       //�ڲ��Ĵ�������
	IIC_Wait_Ack();

	IIC_Stop();                     //����ֹͣ�ź�
}          
//**************************************
//��I2C�豸��ȡһ���ֽ�����
//**************************************
u8 Single_ReadI2C(u8 SlaveAddress,u8 REG_Address)
{
	u8 REG_data=0;
	IIC_Start();    
	
	IIC_Send_Byte(SlaveAddress);    
	IIC_Wait_Ack();

	IIC_Send_Byte(REG_Address);
	IIC_Wait_Ack();

	IIC_Start();
	
	IIC_Send_Byte(SlaveAddress+1);   
	IIC_Wait_Ack();
 
	delay_us(50);
 
	REG_data=IIC_Read_Byte(0);
   
	IIC_Stop();  
     
	return REG_data;
}
 
u32 Read_KS10X(u8 SlaveAddress)
{    
	sum=Single_ReadI2C(SlaveAddress,0x02);             //��8λ
	sum<<=8;
	sum+=Single_ReadI2C(SlaveAddress,0x03);                //��8λ
  return sum;
}
 
void Change_Addr(u8 OldAdddr,u8 NewAddr)
{
	Single_WriteI2C(OldAdddr,0x02,0x9a);             //Ĭ��ԭ��ַ��0x00;
	delay_ms(1);
	Single_WriteI2C(OldAdddr,0x02,0x92);
	delay_ms(1);
	Single_WriteI2C(OldAdddr,0x02,0x9e);
	delay_ms(1);
	Single_WriteI2C(OldAdddr,0x02,NewAddr);
	delay_ms(100);
}
 
//��ȡ��Ӧ��ַ�ĳ���������
float Read_KS10X_Data(u8 SlaveAddress)
{
	float sumx;
	float sum_x;
	Single_WriteI2C(SlaveAddress,0x02,0x0f);  //0x0f̽��ģʽ ����ֵ��79~8655֮�� ����Ϊ0~1500mm ���ʱ��Ϊ9.8ms
	delay_ms(10);
         
	sumx=Read_KS10X(SlaveAddress);          //������һ��������������
	
	if(sumx <= 80)
	{
		sum_x = 0;
	}
	else
	{
		sum_x = (sumx - 80) / (8655 - 80) * 1500;
	}
 
	sum_x  = sumx / 10;
	return sum_x;
}
 
//�޸ĳ�����ģ��ĵ�ַ  ÿ��ģ��ִֻ��һ�Ρ�
void KS10X_Change_Addr_Init(u8 OldAddr,u8 NewAddr)  //�˺�������ʵ��ѡ�������ĵ�ַ
{
	Change_Addr(OldAddr,NewAddr);
	delay_ms(80);
}


