/*
================================================================================
* File Name                 :   lcd12864s.c
* Copyright                 :   Robocon 2013
* 
* CPU                       :   stm32f103
* RTOS                      :   None
*
* Create Date               :   2012/10/19
* Author                    :   Zero
* Abstract Description      :   ���ô��д��䷽ʽ����12864
*...............................................................................
*
* Notes: (1) PIN: PSB = 0 (����)   CS = 1 (Ƭѡ)
*						SCLK -> PI2     SID -> PI1
*
================================================================================
*/


/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "lcd12864s.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
#include "stdio.h"
#include "stdarg.h"
#include "string.h"
#include "ctype.h"
#include "stdlib.h"


/* Private define ------------------------------------------------------------*/
#define u8 unsigned char 
#define uint unsigned int 
 
char LCD_BUF[30];
/* Private macro -------------------------------------------------------------*/
#define LCD_GPIO GPIOI                   
#define LCD_RCC  RCC_AHB1Periph_GPIOI
#define SID_BIT  GPIO_Pin_4
#define SCLK_BIT GPIO_Pin_7
#define SID_0    GPIO_ResetBits(GPIOI,SID_BIT)
#define SID_1    GPIO_SetBits(GPIOI,SID_BIT)
#define SCLK_0   GPIO_ResetBits(GPIOI,SCLK_BIT)
#define SCLK_1   GPIO_SetBits(GPIOI,SCLK_BIT)

/* ��ʱ ***********************************************************************/
void LCD_Delay(u16 ms)
{
	u8 i; 
	while(ms--)	 
		for(i=0;i<6;i++);
}	

/* 12864��ʼ�� ****************************************************************/
void LCD_Init(void)
{
	unsigned long x = 350000*4;
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(LCD_RCC,ENABLE);

    
	GPIO_InitStructure.GPIO_Pin = SID_BIT|SCLK_BIT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_Init(LCD_GPIO,&GPIO_InitStructure); 

//     GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
//     GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
//     GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
//     GPIO_Init(LCD_GPIO,&GPIO_InitStructure);
    
	while( x-- );                                   //����xxx����ʱ����,ɾ����������
	
	LCD_Writecomd(Command_Function_8bits_basic);    //ѡ�����ָ�
	
	LCD_Writecomd(Command_DispControl_DispOn);   	//����ʾ����ꡢ��˸���ر�
	//LCD_Writecomd(Command_DispControl_ReverseOn);	//����ʾ������ʾ��꣬�������˸
	//LCD_Writecomd(Command_DispControl_CursorOn);	//����ʾ����ʾ��꣬����겻��˸
	//LCD_Writecomd(Command_DispControl_AllOn);	   //����ʾ����ꡢ��˸����ʾ   
	
	LCD_Writecomd(Command_ClearScreen);             //�����ʾ�������趨��ַָ��Ϊ00H
	LCD_Writecomd(Command_CursorReturn);            //����λ
	
	//LCD_Writecomd(Command_EntryMode_ACdowm);	   //дһ�ַ���������ʾ���ƶ�,AC�Զ���һ
	//LCD_Writecomd(Command_EntryMode_FrameRight);	//дһ�ַ�������ƽ��,AC�Զ���һ
	LCD_Writecomd(Command_EntryMode_ACup);	         //дһ�ַ���������ʾ���ƶ�,AC�Զ���һ
	//LCD_Writecomd(Command_EntryMode_FrameLeft);	//дһ�ַ�������ƽ��,AC�Զ���һ
}

void LCD_WriteByte(u8 byte)
{
	u8 i;
	SCLK_0;
	for(i=0;i<8;i++)
	{
		if(byte & 0x80)      //�ж�byte�����λ,ȷ��SID��ֵ 
			SID_1;
		else
			SID_0;
		
		SCLK_1;
		LCD_Delay(40);      //��ʱ?
		SCLK_0;
    LCD_Delay(40);
		
		byte = byte<<1;
	}
}

/* дָ�� *********************************************************************/
void LCD_Writecomd(u8 comd)
{
	//LCD_Busy();
	//LCD_Delay(5);
    
	LCD_WriteByte(0xF8);           //11111 RW(0)RS(0) 0
	LCD_WriteByte(comd & 0xF0);    //д��xxxx0000��ʽ
	LCD_WriteByte(0xF0 & comd<<4); //��ִ��<<��д��0000xxxx��ʽ
}

/* д���� *********************************************************************/
void LCD_Writedata(u8 data)
{
   //LCD_Busy();
   //LCD_Delay(5);
    
	LCD_WriteByte(0xFA);           //11111 RW(0)RS(1) 0
	LCD_WriteByte(data & 0xF0);    //д��xxxx0000��ʽ
	LCD_WriteByte(0xF0 & data<<4); //��ִ��<<��д��0000xxxx��ʽ
}

/* ���� ***********************************************************************/
void LCD_Clear(void)
{
	LCD_Writecomd(Command_ClearScreen);
	LCD_Writecomd(Command_CursorReturn);	
}

/* ��λ ***********************************************************************/
void LCD_GotoXY(u8 x,u8 y)
{
	switch(x)
	{
		case 0:LCD_Writecomd(0x80+(y&0x07));break;
		case 1:LCD_Writecomd(0x90+(y&0x07));break;
		case 2:LCD_Writecomd(0x88+(y&0x07));break;
		case 3:LCD_Writecomd(0x98+(y&0x07));break;
		default:break;
	}
}

//==============================================================================
// ������������λ��д����
// ��ڣ�x: ��ʾҪд�������ڵ��е�ַ��ֻ��Ϊ0--3
//       y: ��ʾҪд�������ڵ��е�ַ��ֻ��Ϊ0--7
//		 data: ��ʾҪд���ַ�,ֻ���ǵ��� 
//==============================================================================
void LCD_Putchar(u8 x,u8 y,u8 data)
{
	LCD_GotoXY(x,y);
	LCD_Writedata(data);
}

//==============================================================================
// ������������λ��д�������ַ�,ֻ����ʾһ�� 
// ��ڣ�x: ��ʾҪд�������ڵ��е�ַ��ֻ��Ϊ0--3
//       y: ��ʾҪд�������ڵ��е�ַ��ֻ��Ϊ0--7
//     str: Ҫд�ַ������׵�ַ
//============================================================================== 
void LCD_Putstr(u8 x,u8 y,char *str)
{
	u8 i;
	LCD_GotoXY(x,y);

   for (i=0;i<16 && *str;i++)
   {
   	LCD_Writedata(*str);
   	str++;
	}
}

// void itoa(char *s,int data1)
// {
// 	int data,n=0,i;
// 	char neg[30];
// 	data=data1;
// 	if(data<0)
// 		data=-data;
// 	while(data)
// 	{
// 		neg[n++]=data%10+'0';
// 		data/=10;
// 	}
// 	if(data1<0)
// 		neg[n++]='-';
// 	for(i=n-1;i>=0;i--)
// 	{
// 		*s++=neg[i];
// 	}
// 	*s=0;
// }
// void strcon(char *s,char *b)   //connect string s and string b
// {
// 	while(*s)s++;
// 	while(*b)*s++=*b++;
// 	*s=0;
// }
// void ftoa(char *s,float fdata,int dec) //change double to sting
// {
// 	char icode[30],dcode[30],i,n,flag=0;
// 	long idata,ddata;
// 	if(dec>6)dec=6;
// 	idata=(int)fdata; //��������
// 	
// 	fdata-=idata;
// 	for(i=0;i<dec;i++)
// 		fdata*=10;
// 	if(fdata<0)
// 	{
// 		fdata=-fdata;
// 		flag=1;
// 	}
// 	ddata=(int)(fdata+0.5); // С������
// 	
// 	itoa(icode,idata);
// 	itoa(dcode,ddata);
// 	if(idata==0)
// 	{
// 		if(flag)
// 		{
// 			icode[0]='-';
// 			icode[1]='0';
// 			icode[2]=0;
// 		}
// 		else
// 		{
// 			icode[0]='0';
// 			icode[1]=0;
// 		}
// 	}
// 	n=strlen(dcode);
// 	if(n<dec)
// 	{
// 		for(i=n;i<dec;i++)
// 		{
// 			dcode[i]='0';
// 		}
// 		dcode[i]=0;
// 	}
// 	*s=0;
// 	strcon(s,icode);
// 	n=strlen(s);
// 	s[n]='.';
// 	s[n+1]=0;
// 	strcon(s,dcode);
// 	
// }

// void my_vsprintf(char *s,char *fmt,va_list ap)  //my_vsprintf
// {
// 	int zdata=-1,ddata=6,i;
// 	int ndata;
// 	float fdata;
// 	char neg[30];
// 	for(i=0;i<=30;i++)
// 		 s[i]=0;
// 	while(*fmt)
// 	{
// 		if(*fmt=='%')
// 		{
// 			fmt++;
// 			ddata=6;
// 			while(*fmt!='d'&&*fmt!='f')
// 			{
// 				if(*fmt=='.')
// 					zdata++;
// 				if(*fmt>='0'&&*fmt<='9')
// 				{
// 					if(zdata==-1)
// 						zdata=*fmt-'0';
// 					else
// 						ddata=*fmt-'0';
// 				}
// 				fmt++;
// 			}
// 			switch(*fmt)
// 			{
// 				case 'd':
// 					ndata=va_arg(ap,int);
// 					itoa(neg,ndata);
// 					strcon(s,neg);
// 					while(*s)s++;
// 					break;
// 				case 'f':
// 				    fdata=va_arg(ap,float);
// 				    ftoa(neg,fdata,ddata);
// 				    strcon(s,neg);
// 				    while(*s)s++;
// 				    break;
// 			}
// 			fmt++;
// 		}
// 		else *s++=*fmt++;
// 	}	
// }
void LCD_Printf(u8 x,u8 y,char *fmt,...)  //12864�ĸ�ʽ�����
{
	static u8 last_len[4]={0,0,0,0};
	u8 len,i,yy;
	va_list ap;
	
	va_start(ap,fmt);
	vsprintf(LCD_BUF,fmt,ap);
	va_end(ap);
	
	yy=2*y;
	len=strlen((const char *)LCD_BUF)+yy;
	for(i=len;i<30&&i<last_len[x];i++)
	{
		LCD_BUF[i-yy]=' ';
	}
	LCD_BUF[i-yy]=0;
	LCD_Putstr(x,y,LCD_BUF);
	last_len[x]=len;
}


