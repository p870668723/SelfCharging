#ifndef BANKCOMMON_H
#define BANKCOMMON_H

#ifdef __cplusplus
extern "C" {
#endif
	
#include "stdio.h"
#include "string.h"
#include "stdlib.h"	
	
#include "stm32f4xx.h"
	
#define RC_CH_VALUE_MIN 		((uint16_t)364 )				//摇杆最小值
#define RC_CH_VALUE_OFFSET  ((uint16_t)1024)				//摇杆中间值
#define RC_CH_VALUE_MAX 		((uint16_t)1684)				//摇杆最大值

#define RC_SW_UP 						((uint16_t)1)						//开关  上
#define RC_SW_MID 					((uint16_t)3)						//开关  中
#define RC_SW_DOWN 					((uint16_t)2)						//开关  下
	

#define Buffer_Size					(11*2)  								//SLAM上位机一帧数据的长度	

typedef enum{REMOTE_MODE,SLAM_MODE}CONTROL_MODE;


//遥控器的结构体
typedef struct { 
struct     
{       
	uint16_t ch0;       
	uint16_t ch1;         
	uint16_t ch2;        
	uint16_t ch3;         
	uint8_t  s1;       
	uint8_t  s2;     
}rc;

struct    
{        
	int16_t x;     
	int16_t y;       
	int16_t z;        
	uint8_t press_l;  
	uint8_t press_r;   
}mouse;

struct  
{   
	uint16_t v;   
}key;

}RC_Ctl_t; 



typedef struct sdss
{
	int pre_timestamp[3],timestamp[3];
	double v1,v2,v3;
	double prev1,prev2,prev3;
	double delta_v1,delta_v2,delta_v3;
	char   islock;
}RobotEncoder;




//通用的全局变量
extern CONTROL_MODE Control_Mode;
extern int heartbreak_cnt;
extern RobotEncoder  robot_encoder_data;
	
char findCharPosFromBegin(unsigned char * buffer,unsigned char c);
char findCharPosFromEnd(unsigned char * buffer,unsigned char c);
unsigned char XorCode(unsigned char *buffer,int len);

#ifdef __cplusplus
}
#endif



#endif
