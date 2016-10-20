#ifndef MYFILTER_H
#define MYFILTER_H

/******************************************************
				2�׵�ͨ������˹�˲����ṹ��
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
				һ�����ֵ�ͨ�˲�����ز���
******************************************************/
#define  LPF1_Old                    0.29
#define  LPF1_New										 0.71		


/******************************************************
				          �ⲿ���ú���
******************************************************/
void   ButterWorth_Init(ButterWorth_Struct * bw);
double ButterWorth_Calc(ButterWorth_Struct * bw,double input);
double LowPassFilter_OneOrder(double oldValue,double newValue);


#endif

