#ifndef AVOID_OBSTACLE_H
#define AVOID_OBSTACLE_H

#ifdef __cplusplus
 extern "C" {
#endif



#include "stm32f4xx.h"

#include "mymath.h"
#include "robotmodel.h"
#include "math.h"

void SonarAvoidObstacle(double *sonarData,double *spd);

extern double sonar[3];
	 
#ifdef __cplusplus
}
#endif

#endif





