#ifndef IMU_H
#define IMU_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "mymath.h"
#include "mpu6050.h"
//#include "timer.h"

void IMU_Init(void );
double IMU_Cal(void );
void IMU_GetEuler(double *yaw,double *pitch,double *roll);
double IMU_GetYawRadian(void );
double IMU_GetYawDegree(void );

extern double MAG_YAW;
extern double  Yaw,Pitch,Roll;
extern char   Odom_stable;

#ifdef __cplusplus
}
#endif

#endif

