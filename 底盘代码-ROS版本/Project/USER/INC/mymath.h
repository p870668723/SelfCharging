#ifndef MYMATH_H
#define MYMATH_H

#ifdef __cplusplus
 extern "C" {
#endif

//stm32ͷ�ļ�
#include "stm32f4xx.h"

//C��׼ͷ�ļ�
#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"
	 
//�����Զ�����ѧ��
#include "myfilter.h"
#include "cmatrix.h"

#define PI  	         3.1415926
#define AngleToRad(x)  ((x)/180*PI)				//�ǶȻ�����
#define RadToAngle(x)  ((x)/PI*180)  			//���Ȼ��Ƕ�

#define My_Abs(x)			 ((x)>0?(x):-(x))		//�����ֵ

double invSqrt(double x);
double My_Min(double a,double b);
double My_Max(double a,double b);
double Sign(double x);
double MyAbs(double x);
#ifdef __cplusplus
}
#endif

#endif




