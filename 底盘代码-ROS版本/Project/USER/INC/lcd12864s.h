#ifndef __LCD12864S_H__
#define __LCD12864S_H__
// #include "stdio.h"
// #include "stdarg.h"
#include "stm32f4xx.h"
/*
 *	LCDָ��
 */
#define Command_ClearScreen				    0x01   /*LCD����*/
#define Command_CursorReturn			    0x02   /*LCD����λ*/
#define Command_EntryMode_ACdowm	    0x04   /*DDRAM ��ַ��������AC���� 1,���治��λ*/
#define Command_EntryMode_ACup			  0x06   /*DDRAM ��ַ��������AC���� 1,���治��λ*/
#define Command_EntryMode_FrameRight	0x05   /*������������λ*/
#define Command_EntryMode_FrameLeft		0x07   /*������������λ*/
#define Command_DispControl_DispOff		0x08   /*��ʾ��,�α��,�α�λ�÷��׹�*/
#define Command_DispControl_DispOn		0x0c   /*��ʾ��,�α��,�α�λ�÷��׹�*/
#define Command_DispControl_ReverseOn	0x0d   /*��ʾ��,�α��,�α�λ�÷��׿�*/
#define Command_DispControl_CursorOn	0x0e   /*��ʾ��,�α꿪,�α�λ�÷��׹�*/
#define Command_DispControl_AllOn		  0x0f   /*��ʾ��,�α꿪,�α�λ�÷��׿�*/
#define Command_Function_4bits_basic	0x20   /*4λ���ݣ�����ָ�����*/
#define Command_Function_4bits_exten	0x24   /*4λ���ݣ�����ָ�����*/
#define Command_Function_8bits_basic	0x30   /*8λ���ݣ�����ָ�����*/				
#define Command_Function_8bits_exten	0x34   /*8λ���ݣ�����ָ�����*/
#define Command_CursorShiftLeft			  0x10   /*�������*/
#define Command_CursorShiftRight	    0x14   /*�������*/
#define Command_AllshiftLeft		    	0x18	 /*��ʾ��������*/
#define Command_AllshiftRight		    	0x1c	 /*��ʾ��������*/

void LCD_Init(void);
void LCD_Clear(void);
void LCD_Writecomd(u8 comd);
//void LCD_Writedata(u8 data);
//void LCD_GotoXY(u8 x,u8 y);
void LCD_Putchar(u8 x,u8 y,u8 data);
void LCD_Putstr(u8 x,u8 y,char *str);
void LCD_Printf(u8 x,u8 y,char *fmt,...);


#endif
