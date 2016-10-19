#include "mymath.h"


/**************************实现函数********************************************
*函数原型:double invSqrt(double x) 
*功　　能:快速计算 1/Sqrt(x)
*输入参数:X
*输出参数:X的绝对值
*******************************************************************************/
double invSqrt(double x) 
{
	double halfx = 0.5f * x;
	double y = x;
	uint32_t i = *(uint32_t*)&y;
	i = 0x5f3759df - (i>>1);					//这个很神奇
	y = *(double*)&i;
	y = y * (1.5f	- (halfx * y * y)); //牛顿迭代法 f(x) = 1/x^2 - n
	return y;
}


/**************************实现函数********************************************
*函数原型:double My_Min(double a,double b) 
*功　　能:求最小值
*输入参数:double a,double b
*输出参数:a,b中的较小值
*******************************************************************************/
double My_Min(double a,double b)
{
	if(a>b)return b;
	return a;
}


/**************************实现函数********************************************
*函数原型:double My_Max(double a,double b)
*功　　能:求最大值
*输入参数:double a,double b
*输出参数:a,b中的较大值
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
  * @brief  取数的符号
	* @param  x:待去符号的数
  * @retval 数X的符号 1 表示正 -1表示负
  */
double Sign(double x)
{
	if(x > 0) return 1;
	else return -1;
}










