#include "kalman.h"
#include "stdlib.h"
Kalman_filter kalman;
Kalman_com    kalman_com;
int cmp(const void * a,const void * b)   //��������ıȽϺ���
{
	return *(int *)a - *(int *)b; 
}
void kalman_init(Kalman_filter* kalman)
{
    kalman->x_last=1;
    kalman->p_last=5;     //��������ֵ��ʼ������ν����P��ҪΪ0
    
    kalman->Q=0.0001;     //��������
    kalman->R=0.27;       //�۲�����
    
    kalman->A=1;          //�򻯵�ģ��
    kalman->H=1;
	
	  kalman->com.length=0;
}

double kalman_filter(Kalman_filter *kalman,double input)
{
    kalman->x_last=kalman->A*kalman->x_last;                //������һ�ε�ֵԤ�Ȿ�ε�ֵ
    
    kalman->p_last=kalman->p_last+kalman->Q;                //������һ�ε�Э������㱾�ε�Э����
    
    kalman->kg=kalman->p_last/(kalman->p_last+kalman->R);   //���ݱ��ε�Э�������kalman����
    
    kalman->x_last=kalman->x_last+kalman->kg*(input-kalman->H*kalman->x_last);//������ε�����ֵ
    
    kalman->p_last=(1-kalman->kg)*kalman->p_last;           //������ε�Э����
	
	  kalman->best=kalman->x_last;
    
    return kalman->best;
}

double kalman_com_filter(Kalman_filter *kalman)
{
	int i,count=0;
	long sum=0;
	qsort(kalman->com.data,kalman->com.length,sizeof(int),cmp);
	for(i=kalman->com.length*0.2;i<kalman->com.length*0.8;i++,count++)
	{
		sum+=kalman->com.data[i];
	}
	return sum/=count*1.0;
}
