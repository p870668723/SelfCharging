#ifndef MYMATH_H
#define MYMATH_H

#ifdef __cplusplus
 extern "C" {
#endif

//stm32头文件
#include "stm32f4xx.h"

//C标准头文件
#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "ctype.h"
	 
//其他自定义数学库
#include "myfilter.h"
#include "cmatrix.h"

#define PI  	         3.1415926
#define AngleToRad(x)  ((x)/180*PI)				//角度化弧度
#define RadToAngle(x)  ((x)/PI*180)  			//弧度化角度

#define My_Abs(x)			 ((x)>0?(x):-(x))		//求绝对值

double invSqrt(double x);
double My_Min(double a,double b);
double My_Max(double a,double b);
double Sign(double x);
double MyAbs(double x);
#ifdef __cplusplus
}
#endif

#endif




