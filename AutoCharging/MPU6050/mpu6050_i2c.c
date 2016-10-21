//#include "main.h"
#include "stm32f10x.h"
/*----I2C1----SCL----PB6---*/
/*----I2C1----SDA----PB7---*/

//IO口操作宏定义
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum))  

//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输出 
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输入 

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输出 
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输入 

#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输出 
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输入 
   	   		   
//IO方向设置
#define SDA_IN()  {GPIOA->CRL&=0XFFFFF0FF;GPIOA->CRL|=0x00000800;}
#define SDA_OUT() {GPIOA->CRL&=0XFFFFF0FF;GPIOA->CRL|=0x00000300;}


//IO操作函数	 
#define IIC_SCL    PAout(3) //SCL
#define IIC_SDA    PAout(2) //SDA	 
#define READ_SDA   PAin(2)  //输入SDA 
#define IIC_SDA_Out() SDA_OUT()
#define IIC_SDA_In()  SDA_IN()
#define IIC_SDA_H() IIC_SDA=1
#define IIC_SDA_L() IIC_SDA=0
#define IIC_SCL_H() IIC_SCL=1
#define IIC_SCL_L() IIC_SCL=0
#define IIC_SDA_Read() READ_SDA
void IIC_Delay(unsigned int t)
{
	int i;
	for( i=0;i<t;i++)
	{
		int a = 0;//6
		while(a--);
	}
}

void HEAT_Configuration(void)
{
//	GPIO_InitTypeDef gpio;   

//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
//		
//	gpio.GPIO_Pin = GPIO_Pin_4;	
//	gpio.GPIO_Mode = ;
//	gpio.GPIO_OType = GPIO_OType_PP;
//	gpio.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_Init(GPIOA, &gpio);
//    GPIO_ResetBits(GPIOA,GPIO_Pin_4);//0
}

void IIC_GPIO_Init(void)
{
    GPIO_InitTypeDef   gpio;
    
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	gpio.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_2;
	gpio.GPIO_Mode =GPIO_Mode_Out_OD;
	gpio.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_Init(GPIOA, &gpio);
}

//void IIC_SDA_Out(void)
//{
//GPIOA->CRH&=0XFFFFF0FF;
//GPIOA->CRL|=0X00000300;
//}

//void IIC_SDA_In(void)
//{
//    GPIO_InitTypeDef   gpio;
//    
//	gpio.GPIO_Pin = GPIO_Pin_2;
//    
//    gpio.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//    gpio.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &gpio);
//}

void IIC_Start(void)								  
{
	IIC_SDA_Out();
	IIC_SDA_H();
	IIC_SCL_H();
	IIC_Delay(1);
	IIC_SDA_L();
	IIC_Delay(1);
	IIC_SCL_L();
}

void IIC_Stop(void)
{
	IIC_SDA_Out();
	IIC_SCL_L();
	IIC_SDA_L();
	IIC_Delay(1);
	IIC_SCL_H();
	IIC_SDA_H();
	IIC_Delay(1);
}

void IIC_Ack(u8 re)					     
{
	IIC_SDA_Out();
	if(re)
	   IIC_SDA_H();
	else
	   IIC_SDA_L();
	IIC_SCL_H();
	IIC_Delay(1);
	IIC_SCL_L();
	IIC_Delay(1);
}

int IIC_WaitAck(void)
{
	u16 Out_Time=1000;
    
    IIC_SDA_H();
	IIC_SDA_In();
	IIC_Delay(1);
	IIC_SCL_H();
	IIC_Delay(1);
	while(IIC_SDA_Read())
	{
		if(--Out_Time)
		{
			IIC_Stop();
            printf("error 2A\r\n");
            return 0xff;
		}
	}
	IIC_SCL_L();
    return 0;
}

void IIC_WriteBit(u8 Temp)
{
	u8 i;
	IIC_SDA_Out();
	IIC_SCL_L();
	for(i=0;i<8;i++)
	{
		if(Temp&0x80)
		{
			IIC_SDA_H();
		}
		else
		{
			IIC_SDA_L();
		}
		Temp<<=1;
		IIC_Delay(1);
		IIC_SCL_H();
		IIC_Delay(1);
		IIC_SCL_L();
	}
}

u8 IIC_ReadBit(void)
{
	u8 i,Temp=0;
	IIC_SDA_In();
	for(i=0;i<8;i++)
	{
		IIC_SCL_L();
		IIC_Delay(1);
		IIC_SCL_H();
		Temp<<=1;
		if(IIC_SDA_Read())
		   Temp++;
		IIC_Delay(1);
	}
	IIC_SCL_L();
	return Temp;
}

//写数据，成功返回0，失败返回0xff
int IIC_WriteData(u8 dev_addr,u8 reg_addr,u8 data)
{
	IIC_Start();
    
	IIC_WriteBit(dev_addr);
	if(IIC_WaitAck() == 0xff)
    {
        printf("error 2B\r\n");
        return 0xff;
    }
    
	IIC_WriteBit(reg_addr);
	if(IIC_WaitAck() == 0xff)
    {
        printf("error 2C\r\n");
        return 0xff;
    }

    IIC_WriteBit(data);
    if(IIC_WaitAck() == 0xff)
    {
        printf("error 2D\r\n");
        return 0xff;
    }

	IIC_Stop();
    return 0;
}

//读数据，成功返回0，失败返回0xff
int IIC_ReadData(u8 dev_addr,u8 reg_addr,u8 *pdata,u8 count)
{
    
	u8 i;

    IIC_Start();
	
    IIC_WriteBit(dev_addr);
	if(IIC_WaitAck() == 0xff)
    {
        printf("error 2F\r\n");
        return 0xff;
    }
    
    IIC_WriteBit(reg_addr);
	if(IIC_WaitAck() == 0xff)
    {
        printf("error 2G\r\n");
        return 0xff;
    }
	
    IIC_Start();
    
    IIC_WriteBit(dev_addr+1);
	if(IIC_WaitAck() == 0xff)
    {
        printf("error 2H\r\n");
        return 0xff;
    }
    
    for(i=0;i<(count-1);i++)
    {
        *pdata=IIC_ReadBit();
        IIC_Ack(0);
        pdata++;
    }

    *pdata=IIC_ReadBit();
    IIC_Ack(1); 
    IIC_Stop(); 
    
    return 0;    
}
