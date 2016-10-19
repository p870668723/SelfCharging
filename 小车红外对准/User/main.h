#ifndef __MAIN_H__
#define __MAIN_H__

#include "stm32f10x.h"
#include <stdio.h>
#include <stdlib.h>
#include "stm32f10x_it.h"
#include "encoder.h"
#include "led.h"
#include "pwm.h"
#include "delay.h"
#include "uart.h"
#include "can.h"
#include "ad.h"
#include "flash.h"
#include "key.h"
#include "iwdg.h"
#include "DEBUG.h"
#include "control.h"
#include "BSP.H"
#include "math.H"
#include "mpu6050_i2c.h"
#include "mpu6050_interrupt.h"
#include "mpu6050_driver.h"
#include "mpu6050_process.h"
#include "HMC5883L.h"
#include "time.h"
#include "stmflash.h"
#include "beep.h"
#include "ldc1000.h"
#include "app.h"
#define abs(x) ((x)>0? (x):(-(x)))
//#define set_speed_max  650
#define PWM_Period     8000
#define POWER_Period   20
#define SPEED_LIMIT   200
#endif
