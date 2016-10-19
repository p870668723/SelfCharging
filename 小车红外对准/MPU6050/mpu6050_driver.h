#ifndef __MPU6050_DRIVER_H__
#define __MPU6050_DRIVER_H__

typedef struct{
	short x;
	short y;
	short z;
}S_INT16_XYZ;


typedef struct{
	float x;
	float y;
	float z;}S_FLOAT_XYZ;

typedef struct __MPU6050_RAW_Data__
{
    short Accel_X;  //�Ĵ���ԭ��X����ٶȱ�ʾֵ
    short Accel_Y;  //�Ĵ���ԭ��Y����ٶȱ�ʾֵ
    short Accel_Z;  //�Ĵ���ԭ��Z����ٶȱ�ʾֵ
    short Temp;     //�Ĵ���ԭ���¶ȱ�ʾֵ
    short Gyro_X;   //�Ĵ���ԭ��X�������Ǳ�ʾֵ
    short Gyro_Y;   //�Ĵ���ԭ��Y�������Ǳ�ʾֵ
    short Gyro_Z;   //�Ĵ���ԭ��Z�������Ǳ�ʾֵ
}MPU6050_RAW_DATA;

typedef struct __MPU6050_REAL_Data__
{
    float Accel_X;  //ת����ʵ�ʵ�X����ٶȣ�
    float Accel_Y;  //ת����ʵ�ʵ�Y����ٶȣ�
    float Accel_Z;  //ת����ʵ�ʵ�Z����ٶȣ�
    float Temp;     //ת����ʵ�ʵ��¶ȣ���λΪ���϶�
    float Gyro_X;   //ת����ʵ�ʵ�X��Ǽ��ٶȣ�
    float Gyro_Y;   //ת����ʵ�ʵ�Y��Ǽ��ٶȣ�
    float Gyro_Z;   //ת����ʵ�ʵ�Z��Ǽ��ٶ�
}MPU6050_REAL_DATA;

extern MPU6050_RAW_DATA    MPU6050_Raw_Data; 
extern MPU6050_REAL_DATA   MPU6050_Real_Data;
extern S_INT16_XYZ		MPU6050_ACC_LAST,MPU6050_GYRO_LAST,HMC5883L_MAG_LAST;		
extern S_INT16_XYZ		GYRO_OFFSET,ACC_OFFSET,MAG_OFFSET,MAG_AVG,GYRO_AVG;			
extern unsigned char	GYRO_OFFSET_OK;
extern unsigned char	ACC_OFFSET_OK; 
int MPU6050_Initialization(void);
int MPU6050_ReadData(void);
void MPU6050_Gyro_calibration(void);

#endif
