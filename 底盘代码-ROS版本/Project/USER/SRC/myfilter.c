#include "myfilter.h"
#include "math.h"
#include "mymath.h"



/*******************************************************二阶低通巴特沃斯滤波器***************************************************************************/

/**************************实现函数********************************************
*函数原型:	 void ButterWorth_Init(ButterWorth_Struct * bw)
*功　　能:	 巴特沃斯低通滤波器初始化函数
*输入参数：  ButterWorth_Struct * bw
						 指向巴特沃斯结构体的指针
*输出参数：  无
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


/**************************实现函数********************************************
*函数原型:	 float ButterWorth_Calc(ButterWorth_Struct * bw,float input)
*功　　能:	 巴特沃斯低通滤波器滤波函数
*输入参数：  ButterWorth_Struct * bw
						 指向巴特沃斯结构体的指针
						 float input
						 待滤波的数据
*输出参数：  滤波过后的数据
******************************************************************************/
double ButterWorth_Calc(ButterWorth_Struct * bw,double input)
{
	bw->output[2] = bw->a*input + bw->b*bw->output[1] - bw->c*bw->output[0];
	bw->output[0] = bw->output[1];
	bw->output[1] = bw->output[2];
	return bw->output[2];
}


/*******************************************************一阶低通滤波器***************************************************************************/
double LowPassFilter_OneOrder(double oldValue,double newValue)
{
	double LPF_Out;
	LPF_Out = oldValue * LPF1_Old + newValue * LPF1_New;
	return LPF_Out;
}

/*******************************************************一阶卡尔曼滤波器***************************************************************************/







