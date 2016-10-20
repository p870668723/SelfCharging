#include "I2C.h"
#include "systick.h"


#define I2C_Delay()    systick_us(15)

void I2C2_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOH,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOH,&GPIO_InitStructure);
	
}
 
void IIC_Start(void)
{
    SDA_HIGH();//SDA=1;        
    SCL_HIGH();//SCL=1;
    I2C_Delay();
 
    SDA_LOW();//SDA=0;                   //START:when CLK is high,DATA change form high to low 
    I2C_Delay();
 
    SCL_LOW();//SCL=0;                   //ǯסI2C���ߣ�׼�����ͻ�������� 
 
}     
//����IICֹͣ�ź�
void IIC_Stop(void)
{
   // SDA_OUT();                  //sda�����
    SCL_LOW();//SCL=0;
    SDA_LOW();//SDA=0;
    I2C_Delay(); 
    SCL_HIGH();//SCL=1;                      //STOP:when CLK is high DATA change form low to high 
    SDA_HIGH();//SDA=1;                      //����I2C���߽����ź�
    I2C_Delay();                           
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 IIC_Wait_Ack(void)
{
    u8 ucErrTime=0;
    //SDA_IN();      //SDA����Ϊ����  
    SDA_HIGH();systick_us(6);     
    SCL_HIGH();systick_us(6);   
     
    while(SDA_READ)
    {
        ucErrTime++;
        if(ucErrTime>250)
        {
            IIC_Stop();
            return 1;
        }
    }
    SCL_LOW();//SCL=0;                             //ʱ�����0     
    return 0;  
} 
//����ACKӦ��
void IIC_Ack(void)
{
    SCL_LOW();
   // SDA_OUT();
    SDA_LOW();
    I2C_Delay();
    SCL_HIGH();
    I2C_Delay();
    SCL_LOW();
}
//������ACKӦ��          
void IIC_NAck(void)
{
    SCL_LOW();
   // SDA_OUT();
    SDA_HIGH();
    I2C_Delay();
    SCL_HIGH();
    I2C_Delay();
    SCL_LOW();
}                                        
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��           
void IIC_Send_Byte(u8 txd)
{                        
    u8 t;   
   // SDA_OUT();      
    SCL_LOW();                                    //����ʱ�ӿ�ʼ���ݴ���
     
    for(t=0;t<8;t++)
    {              
			  if(txd & 0x80)
					SDA_HIGH();
				else
					SDA_LOW();
        //SDA=(txd&0x80)>>7;
        txd<<=1;    
        I2C_Delay();                          //��TEA5767��������ʱ���Ǳ����
        SCL_HIGH();
        I2C_Delay(); 
        SCL_LOW();  
        I2C_Delay();
    }    
}       
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 IIC_Read_Byte(unsigned char ack)
{
    unsigned char i,receive=0;
   // SDA_IN();                                //SDA����Ϊ����
 
    for(i=0;i<8;i++ )
    {
        SCL_LOW(); 
        I2C_Delay();
        SCL_HIGH();
        receive<<=1;
        if(SDA_READ)receive++;   
        systick_us(5); 
    }                    
     
    if (!ack)
        IIC_NAck();                         //����nACK
    else
        IIC_Ack();                          //����ACK  
     
    return receive;
}

