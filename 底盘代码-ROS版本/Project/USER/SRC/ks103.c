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
	IIC_Start();                    //起始信号
	
	IIC_Send_Byte(SlaveAddress);   //发送设备地址+写信号
	IIC_Wait_Ack();
 
	IIC_Send_Byte(REG_Address);    //内部寄存器地址
	IIC_Wait_Ack();              

	IIC_Send_Byte(REG_data);       //内部寄存器数据
	IIC_Wait_Ack();

	IIC_Stop();                     //发送停止信号
}          
//**************************************
//从I2C设备读取一个字节数据
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
	sum=Single_ReadI2C(SlaveAddress,0x02);             //高8位
	sum<<=8;
	sum+=Single_ReadI2C(SlaveAddress,0x03);                //低8位
  return sum;
}
 
void Change_Addr(u8 OldAdddr,u8 NewAddr)
{
	Single_WriteI2C(OldAdddr,0x02,0x9a);             //默认原地址是0x00;
	delay_ms(1);
	Single_WriteI2C(OldAdddr,0x02,0x92);
	delay_ms(1);
	Single_WriteI2C(OldAdddr,0x02,0x9e);
	delay_ms(1);
	Single_WriteI2C(OldAdddr,0x02,NewAddr);
	delay_ms(100);
}
 
//读取相应地址的超声波数据
float Read_KS10X_Data(u8 SlaveAddress)
{
	float sumx;
	float sum_x;
	Single_WriteI2C(SlaveAddress,0x02,0x0f);  //0x0f探测模式 返回值在79~8655之间 量程为0~1500mm 最大时间为9.8ms
	delay_ms(10);
         
	sumx=Read_KS10X(SlaveAddress);          //读出第一个超声波的数据
	
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
 
//修改超声波模块的地址  每个模块只执行一次。
void KS10X_Change_Addr_Init(u8 OldAddr,u8 NewAddr)  //此函数用来实现选择超声波的地址
{
	Change_Addr(OldAddr,NewAddr);
	delay_ms(80);
}


