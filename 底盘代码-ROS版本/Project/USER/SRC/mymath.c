#include "mymath.h"


/**************************ʵ�ֺ���********************************************
*����ԭ��:double invSqrt(double x) 
*��������:���ټ��� 1/Sqrt(x)
*�������:X
*�������:X�ľ���ֵ
*******************************************************************************/
double invSqrt(double x) 
{
	double halfx = 0.5f * x;
	double y = x;
	uint32_t i = *(uint32_t*)&y;
	i = 0x5f3759df - (i>>1);					//���������
	y = *(double*)&i;
	y = y * (1.5f	- (halfx * y * y)); //ţ�ٵ����� f(x) = 1/x^2 - n
	return y;
}


/**************************ʵ�ֺ���********************************************
*����ԭ��:double My_Min(double a,double b) 
*��������:����Сֵ
*�������:double a,double b
*�������:a,b�еĽ�Сֵ
*******************************************************************************/
double My_Min(double a,double b)
{
	if(a>b)return b;
	return a;
}


/**************************ʵ�ֺ���********************************************
*����ԭ��:double My_Max(double a,double b)
*��������:�����ֵ
*�������:double a,double b
*�������:a,b�еĽϴ�ֵ
*******************************************************************************/
double My_Max(double a,double b)
{
	if(a>b)return a;
	return b;
}

double MyAbs(double x)
{
	if(x > 0)return x;
	return -x;
}


/**
  * @brief  ȡ���ķ���
	* @param  x:��ȥ���ŵ���
  * @retval ��X�ķ��� 1 ��ʾ�� -1��ʾ��
  */
double Sign(double x)
{
	if(x > 0) return 1;
	else return -1;
}










