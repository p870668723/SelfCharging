#ifndef MYFILTER_H
#define MYFILTER_H

/******************************************************
				2阶低通巴特沃斯滤波器结构体
******************************************************/
#define BUTTERWORTH_SAMPLERATE			200
#define BUTTERWORTH_CUTOFFRATE			30
typedef struct  ss 
{
	float fs,fc;
	float m,a,b,c;
	float output[3];
}ButterWorth_Struct;


/******************************************************
				一阶数字低通滤波器相关参数
******************************************************/
#define  LPF1_Old                    0.29
#define  LPF1_New										 0.71		


/******************************************************
				          外部引用函数
******************************************************/
void   ButterWorth_Init(ButterWorth_Struct * bw);
double ButterWorth_Calc(ButterWorth_Struct * bw,double input);
double LowPassFilter_OneOrder(double oldValue,double newValue);


#endif

