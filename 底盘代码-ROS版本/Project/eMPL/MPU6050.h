#ifndef MPU9150_H
#define MPU9150_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f4xx.h"

#include "mymath.h"
#include "timer.h"

#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "mpu_i2c.h"
	 
#include "OLED.h"
#include "kalman.h"


#define AK8975_I2C_ADDR     ((u8)0x0C)
#define AK8975_Device_ID    ((u8)0x48)

#define	AK8975_WIA          ((u8)0x00)
#define	AK8975_INFO         ((u8)0x01)
#define	AK8975_ST1          ((u8)0x02)
#define	AK8975_HXL          ((u8)0x03)
#define	AK8975_HXH          ((u8)0x04)
#define	AK8975_HYL          ((u8)0x05)
#define	AK8975_HYH          ((u8)0x06)
#define	AK8975_HZL          ((u8)0x07)
#define	AK8975_HZH          ((u8)0x08)
#define	AK8975_ST2          ((u8)0x09)
#define	AK8975_CNTL          ((u8)0x0A)
#define	AK8975_RSV          ((u8)0x0B)
#define	AK8975_ASTC         ((u8)0x0C)
#define	AK8975_TS1          ((u8)0x0D)
#define	AK8975_TS2          ((u8)0x0E)
#define	AK8975_I2CDIS       ((u8)0x0F)
#define	AK8975_ASAX         ((u8)0x10)
#define	AK8975_ASAY         ((u8)0x11)
#define	AK8975_ASAZ         ((u8)0x12)




#define MPU6050_I2C_ADDR            ((u8)0xD0)
#define MPU6050_Device_ID           ((u8)0x68)

#define	MPU6050_SELF_TEST_X         ((u8)0x0D)
#define	MPU6050_SELF_TEST_Y         ((u8)0x0E)
#define	MPU6050_SELF_TEST_Z         ((u8)0x0F)
#define	MPU6050_SELF_TEST_A         ((u8)0x10)
#define	MPU6050_SMPLRT_DIV          ((u8)0x19)
#define	MPU6050_CONFIG              ((u8)0x1A)
#define	MPU6050_GYRO_CONFIG         ((u8)0x1B)
#define	MPU6050_ACCEL_CONFIG        ((u8)0x1C)
#define	MPU6050_MOT_THR             ((u8)0x1F)
#define	MPU6050_FIFO_EN             ((u8)0x23)
#define	MPU6050_I2C_MST_CTRL        ((u8)0x24)
#define	MPU6050_I2C_SLV0_ADDR       ((u8)0x25)
#define	MPU6050_I2C_SLV0_REG        ((u8)0x26)
#define	MPU6050_I2C_SLV0_CTRL       ((u8)0x27)
#define	MPU6050_I2C_SLV1_ADDR       ((u8)0x28)
#define	MPU6050_I2C_SLV1_REG        ((u8)0x29)
#define	MPU6050_I2C_SLV1_CTRL       ((u8)0x2A)
#define	MPU6050_I2C_SLV2_ADDR       ((u8)0x2B)
#define	MPU6050_I2C_SLV2_REG        ((u8)0x2C)
#define	MPU6050_I2C_SLV2_CTRL       ((u8)0x2D)
#define	MPU6050_I2C_SLV3_ADDR       ((u8)0x2E)
#define	MPU6050_I2C_SLV3_REG        ((u8)0x2F)
#define	MPU6050_I2C_SLV3_CTRL       ((u8)0x30)
#define	MPU6050_I2C_SLV4_ADDR       ((u8)0x31)
#define	MPU6050_I2C_SLV4_REG        ((u8)0x32)
#define	MPU6050_I2C_SLV4_DO         ((u8)0x33)
#define	MPU6050_I2C_SLV4_CTRL       ((u8)0x34)
#define	MPU6050_I2C_SLV4_DI         ((u8)0x35)
#define	MPU6050_I2C_MST_STATUS      ((u8)0x36)
#define	MPU6050_INT_PIN_CFG         ((u8)0x37)
#define	MPU6050_INT_ENABLE          ((u8)0x38)
#define	MPU6050_INT_STATUS          ((u8)0x3A)
#define	MPU6050_ACCEL_XOUT_H        ((u8)0x3B)
#define	MPU6050_ACCEL_XOUT_L        ((u8)0x3C)
#define	MPU6050_ACCEL_YOUT_H        ((u8)0x3D)
#define	MPU6050_ACCEL_YOUT_L        ((u8)0x3E)
#define	MPU6050_ACCEL_ZOUT_H        ((u8)0x3F)
#define	MPU6050_ACCEL_ZOUT_L        ((u8)0x40)
#define	MPU6050_TEMP_OUT_H          ((u8)0x41)
#define	MPU6050_TEMP_OUT_L          ((u8)0x42)
#define	MPU6050_GYRO_XOUT_H         ((u8)0x43)
#define	MPU6050_GYRO_XOUT_L         ((u8)0x44)
#define	MPU6050_GYRO_YOUT_H         ((u8)0x45)
#define	MPU6050_GYRO_YOUT_L         ((u8)0x46)
#define	MPU6050_GYRO_ZOUT_H         ((u8)0x47)
#define	MPU6050_GYRO_ZOUT_L         ((u8)0x48)
#define	MPU6050_EXT_SENS_DATA_00    ((u8)0x49)
#define	MPU6050_EXT_SENS_DATA_01    ((u8)0x4A)
#define	MPU6050_EXT_SENS_DATA_02    ((u8)0x4B)
#define	MPU6050_EXT_SENS_DATA_03    ((u8)0x4C)
#define	MPU6050_EXT_SENS_DATA_04    ((u8)0x4D)
#define	MPU6050_EXT_SENS_DATA_05    ((u8)0x4E)
#define	MPU6050_EXT_SENS_DATA_06    ((u8)0x4F)
#define	MPU6050_EXT_SENS_DATA_07    ((u8)0x50)
#define	MPU6050_EXT_SENS_DATA_08    ((u8)0x51)
#define	MPU6050_EXT_SENS_DATA_09    ((u8)0x52)
#define	MPU6050_EXT_SENS_DATA_10    ((u8)0x53)
#define	MPU6050_EXT_SENS_DATA_11    ((u8)0x54)
#define	MPU6050_EXT_SENS_DATA_12    ((u8)0x55)
#define	MPU6050_EXT_SENS_DATA_13    ((u8)0x56)
#define	MPU6050_EXT_SENS_DATA_14    ((u8)0x57)
#define	MPU6050_EXT_SENS_DATA_15    ((u8)0x58)
#define	MPU6050_EXT_SENS_DATA_16    ((u8)0x59)
#define	MPU6050_EXT_SENS_DATA_17    ((u8)0x5A)
#define	MPU6050_EXT_SENS_DATA_18    ((u8)0x5B)
#define	MPU6050_EXT_SENS_DATA_19    ((u8)0x5C)
#define	MPU6050_EXT_SENS_DATA_20    ((u8)0x5D)
#define	MPU6050_EXT_SENS_DATA_21    ((u8)0x5E)
#define	MPU6050_EXT_SENS_DATA_22    ((u8)0x5F)
#define	MPU6050_EXT_SENS_DATA_23    ((u8)0x60)
#define	MPU6050_I2C_SLV0_DO         ((u8)0x63)
#define	MPU6050_I2C_SLV1_DO         ((u8)0x64)
#define	MPU6050_I2C_SLV2_DO         ((u8)0x65)
#define	MPU6050_I2C_SLV3_DO         ((u8)0x66)
#define	MPU6050_I2C_MST_DELAY_CTRL  ((u8)0x67)
#define	MPU6050_SIGNAL_PATH_RESET   ((u8)0x68)
#define	MPU6050_MOT_DETECT_CTRL     ((u8)0x69)
#define	MPU6050_USER_CTRL           ((u8)0x6A)
#define	MPU6050_PWR_MGMT_1          ((u8)0x6B)
#define	MPU6050_PWR_MGMT_2          ((u8)0x6C)
#define	MPU6050_FIFO_COUNTH         ((u8)0x72)
#define	MPU6050_FIFO_COUNTL         ((u8)0x73)
#define	MPU6050_FIFO_R_W            ((u8)0x74)
#define MPU6050_WHO_AM_I            ((u8)0x75)	// 0x68


#define Gyro_250_Scale_Factor   131.0f
#define Gyro_500_Scale_Factor   65.5f
#define Gyro_1000_Scale_Factor  32.8f
#define Gyro_2000_Scale_Factor  16.4f
#define Accel_2_Scale_Factor    16384.0f
#define Accel_4_Scale_Factor    8192.0f
#define Accel_8_Scale_Factor    4096.0f
#define Accel_16_Scale_Factor   2048.0f

extern u8 mpu6050_initflag;

extern double gx_bias,gy_bias,gz_bias,ax_bias,ay_bias,az_bias;
extern double gx_rate,gy_rate,gz_rate;
extern char Odom_stable;


void MPU6050_GetBias(void );
double MPU6050_GetData(double gyro[], double acc[]);
void MPU6050_Init(void );

#ifdef __cplusplus
}
#endif

#endif

