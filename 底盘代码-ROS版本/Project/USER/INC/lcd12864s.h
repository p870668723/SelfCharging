#ifndef __LCD12864S_H__
#define __LCD12864S_H__
// #include "stdio.h"
// #include "stdarg.h"
#include "stm32f4xx.h"
/*
 *	LCD指令
 */
#define Command_ClearScreen				    0x01   /*LCD清屏*/
#define Command_CursorReturn			    0x02   /*LCD光标归位*/
#define Command_EntryMode_ACdowm	    0x04   /*DDRAM 地址计数器（AC）减 1,画面不移位*/
#define Command_EntryMode_ACup			  0x06   /*DDRAM 地址计数器（AC）加 1,画面不移位*/
#define Command_EntryMode_FrameRight	0x05   /*画面整体右移位*/
#define Command_EntryMode_FrameLeft		0x07   /*画面整体左移位*/
#define Command_DispControl_DispOff		0x08   /*显示关,游标关,游标位置反白关*/
#define Command_DispControl_DispOn		0x0c   /*显示开,游标关,游标位置反白关*/
#define Command_DispControl_ReverseOn	0x0d   /*显示开,游标关,游标位置反白开*/
#define Command_DispControl_CursorOn	0x0e   /*显示开,游标开,游标位置反白关*/
#define Command_DispControl_AllOn		  0x0f   /*显示开,游标开,游标位置反白开*/
#define Command_Function_4bits_basic	0x20   /*4位数据，基本指令操作*/
#define Command_Function_4bits_exten	0x24   /*4位数据，扩充指令操作*/
#define Command_Function_8bits_basic	0x30   /*8位数据，基本指令操作*/				
#define Command_Function_8bits_exten	0x34   /*8位数据，扩充指令操作*/
#define Command_CursorShiftLeft			  0x10   /*光标左移*/
#define Command_CursorShiftRight	    0x14   /*光标右移*/
#define Command_AllshiftLeft		    	0x18	 /*显示整体左移*/
#define Command_AllshiftRight		    	0x1c	 /*显示整体右移*/

void LCD_Init(void);
void LCD_Clear(void);
void LCD_Writecomd(u8 comd);
//void LCD_Writedata(u8 data);
//void LCD_GotoXY(u8 x,u8 y);
void LCD_Putchar(u8 x,u8 y,u8 data);
void LCD_Putstr(u8 x,u8 y,char *str);
void LCD_Printf(u8 x,u8 y,char *fmt,...);


#endif
