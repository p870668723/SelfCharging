#include "myfilter.h"
#include "math.h"
#include "mymath.h"



/*******************************************************���׵�ͨ������˹�˲���***************************************************************************/

/**************************ʵ�ֺ���********************************************
*����ԭ��:	 void ButterWorth_Init(ButterWorth_Struct * bw)
*��������:	 ������˹��ͨ�˲�����ʼ������
*���������  ButterWorth_Struct * bw
						 ָ�������˹�ṹ���ָ��
*���������  ��
******************************************************************************/
void ButterWorth_Init(ButterWorth_Struct * bw)
{
	bw->fs = BUTTERWORTH_SAMPLERATE;
	bw->fc = BUTTERWORTH_CUTOFFRATE;
	bw->m  = bw->fs*bw->fs + 4*PI*bw->fs*bw->fc + 4*PI*PI*bw->fc*bw->fc;
	bw->a  = 4*PI*PI*bw->fc*bw->fc / bw->m;
	bw->b  = (2*bw->fs*bw->fs + 4*PI*bw->fs*bw->fc) / bw->m;
	bw->c  = bw->fs*bw->fs / bw->m;
	bw->output[0] = bw->output[1] = bw->output[2] = 0;
}


/**************************ʵ�ֺ���********************************************
*����ԭ��:	 float ButterWorth_Calc(ButterWorth_Struct * bw,float input)
*��������:	 ������˹��ͨ�˲����˲�����
*���������  ButterWorth_Struct * bw
						 ָ�������˹�ṹ���ָ��
						 float input
						 ���˲�������
*���������  �˲����������
******************************************************************************/
double ButterWorth_Calc(ButterWorth_Struct * bw,double input)
{
	bw->output[2] = bw->a*input + bw->b*bw->output[1] - bw->c*bw->output[0];
	bw->output[0] = bw->output[1];
	bw->output[1] = bw->output[2];
	return bw->output[2];
}


/*******************************************************һ�׵�ͨ�˲���***************************************************************************/
double LowPassFilter_OneOrder(double oldValue,double newValue)
{
	double LPF_Out;
	LPF_Out = oldValue * LPF1_Old + newValue * LPF1_New;
	return LPF_Out;
}

/*******************************************************һ�׿������˲���***************************************************************************/







