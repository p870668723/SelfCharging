#include "oled.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stdio.h"
#include "stdarg.h"
#include "string.h"
#include "oledlib.h"
u8 OLED_BUF[100];
void OLED_IO_Init()
{
	GPIO_InitTypeDef gpio;
	RCC_AHB1PeriphClockCmd(OLED_CLOCK,ENABLE);
	
	gpio.GPIO_Pin=SDA_BIT;
	gpio.GPIO_Mode=GPIO_Mode_OUT;
	gpio.GPIO_OType=GPIO_OType_PP;
	gpio.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(SDA_GPIO,&gpio);
	
	gpio.GPIO_Pin=SCK_BIT;
	GPIO_Init(SCK_GPIO,&gpio);
	
	gpio.GPIO_Pin=RST_BIT;
	GPIO_Init(RST_GPIO,&gpio);
	
	gpio.GPIO_Pin=DC_BIT;
	GPIO_Init(DC_GPIO,&gpio);
}

void OLED_Delayus(int t)
{
   while(t--);
}
void OLED_Delayms(int time)
{
	unsigned short i, j;
	for(;time>0;time--)
		 for(j = 0; j < 10; j++)		
			 for(i = 0; i < 1000; i++);
}
void OLED_WriteByte(u8 byte)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		if(byte&0x80)
			 OLED_SDA_H;
		else 
			 OLED_SDA_L;
		OLED_SCK_H;
		OLED_SCK_L;
		byte<<=1;
	}
}
void OLED_WriteCmd(u8 cmd)
{
	OLED_DC_L;
	OLED_WriteByte(cmd);
}
void OLED_WriteDat(u8 dat)
{
	OLED_DC_H;
	OLED_WriteByte(dat);
}
void OLED_WriteReg(u8 num,...)
{
	u8 cmd;
	va_list ap;
	va_start(ap,num);
	while(num--)
	{
		cmd=va_arg(ap,int);
		OLED_WriteCmd(cmd);
  }
	va_end(ap);
}

void OLED_Goto(u8 row,u8 col) 
{
	row%=8;
	col%=128;
	OLED_WriteReg(1,0xb0+row);
	OLED_WriteReg(1,0x00+col%16);
	OLED_WriteReg(1,0x10+col/16);
}
void OLED_Clear()
{
	u8 i,j;
	for(i=0;i<8;i++)
	{
		OLED_Goto(i,0);
		for(j=0;j<128;j++)
			OLED_WriteDat(0);
	}
}
void OLED_HorizontalScroll(u8 start,u8 end,u8 speed)
{
	speed%=8;
	start%=8;
	end%=8;
	if(end<start)
		 end=start;
	OLED_WriteReg(1,0x2e); //disable
	OLED_WriteReg(7,0x26,0x00,start*2,speed,end*2+1,0x00,0xff);
	OLED_WriteReg(1,0x2f); //enable
}
void OLED_ShowEnglish(u8 row,u8 col,u8 *s) //row =0~3,col=0~15
{
	u8 i,c;
	row%=8;
	col%=16;
	row*=2;
	col*=8;
	while(*s)
	{
		c=*s-32;
		OLED_Goto(row,col);
		for(i=0;i<8;i++)
		{
			OLED_WriteDat(F8X16[c*16+i]);
		}
		OLED_Goto(row+1,col);
		for(i=0;i<8;i++)
		{
			OLED_WriteDat(F8X16[c*16+i+8]);
		}
		s++;
		col+=8;
	}
}
void OLED_Printf(u8 row,u8 col,char *fmt,...)
{
	static u8 last_len[4]={0,0,0,0};
	u8 len,i,yy;
	va_list ap;

	if(row>7||col>15)return ;
	
	va_start(ap,fmt);
	vsprintf((char *)OLED_BUF,fmt,ap);
	va_end(ap);
	
	yy=2*col;
	len=strlen((const char *)OLED_BUF)+yy;
	for(i=len;i<last_len[row];i++)
	{
		OLED_BUF[i-yy]=' ';
	}
	OLED_BUF[i-yy]=0;
	OLED_ShowEnglish(row,col,OLED_BUF);
	last_len[row]=len;
}
void OLED_Init()
{

	OLED_IO_Init();
	OLED_SCK_L;
	
    OLED_RST_L;
	OLED_Delayms(200);
	OLED_RST_H;
	
	OLED_WriteReg(2,0x20,0x02); //page addressing mode
	OLED_WriteReg(3,0x21,0x00,0x7f); //col_adress 0~127
	OLED_WriteReg(3,0x22,0x00,0x07); //page_adress 0~7
	OLED_WriteReg(2,0xa8,0x3f); //MulRatio
	OLED_WriteReg(2,0xd3,0x00); //display offset
	OLED_WriteReg(1,0x40);      //display start line
	OLED_WriteReg(1,0xa1);      //segment remap     
	OLED_WriteReg(1,0xc8);      //scan direction    
	OLED_WriteReg(2,0xda,0x12); //hard configuration
	OLED_WriteReg(2,0x81,0x7f); //constrast 
	OLED_WriteReg(1,0xa4);      //entire display on
	OLED_WriteReg(1,0xa6);      //normal display
	OLED_WriteReg(2,0xd5,0x80); //display clk,frequently
	OLED_WriteReg(2,0x8d,0x14);//charge pump
	OLED_WriteReg(1,0xaf);      //display ON
	
	
	OLED_Clear();
}


