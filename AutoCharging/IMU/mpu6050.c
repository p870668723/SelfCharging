#include "MPU6050.h"
#include "mymath.h"
#include "kalman.h"
#include "systick.h"
//#include "board.h"


#define USARTX   USART1

#define MAX_NUM       100
#define ONE_LP_B      0.29
#define ONE_LP_A      0.71
#define THRO          8     //积分阀值

#define ACC_NUM       20


//#define  COMPASS  
double gx_rate,gy_rate,gz_rate,ax_rate,ay_rate,az_rate;
double gx_bias,gy_bias,gz_bias,ax_bias,ay_bias,az_bias;
double gz_sum = 0,gy_sum = 0,gx_sum = 0,ax_sum = 0,ay_sum = 0,az_sum = 0;
double gz_biasdata[MAX_NUM];
double Ax[2],Ay[2],Az[2],Gx[2],Gy[2],Gz[2];
double acc_array[3][20],acc_sum[3]={0,0,0};
int acc_index = 0;
short Rdata[3];


char Odom_stable = 0;

int sample_cnt = 0;
u8 mpu6050_initflag = 0;

/*
设置零点
用来更新z轴陀螺仪的零点。
以及计算初始的四元数(通过加速度计的初始值)
*/
void MPU6050_GetBias(void )
{
	 int index;
	
	 mpu_get_accel_reg(Rdata,0);
	 Ax[0] = Rdata[0];
	 Ay[0] = Rdata[1];
	 Az[0] = Rdata[2];
	 
	 mpu_get_gyro_reg(Rdata,0);
	 Gx[0] = Rdata[0];
	 Gy[0] = Rdata[1];
	 gz_biasdata[0] = Rdata[2];


	 ax_sum += Ax[0];
	 ay_sum += Ay[0];
	 az_sum += Az[0];
	 gx_sum += Gx[0];
	 gy_sum += Gy[0];
	 gz_sum += gz_biasdata[0];
	
	 for(index = 1; index < MAX_NUM ;index++)
	 {
		  systick_ms(10);
		  
		  mpu_get_accel_reg(Rdata,0);
		  Ax[1] = ONE_LP_B*Ax[0] + ONE_LP_A*Rdata[0];
		  Ay[1] = ONE_LP_B*Ay[0] + ONE_LP_A*Rdata[1];
		  Az[1] = ONE_LP_B*Az[0] + ONE_LP_A*Rdata[2];
		 
			if(acc_index < ACC_NUM)
			{
				acc_array[0][acc_index] = Ax[1];
				acc_sum[0] += Ax[1];
				
				acc_array[1][acc_index] = Ay[1];
				acc_sum[1] += Ay[1];
				
				acc_array[2][acc_index] = Az[1];
				acc_sum[2] += Az[1];
			}
		 
		  mpu_get_gyro_reg(Rdata,0);
		  Gx[1] = ONE_LP_B*Gx[0] + ONE_LP_A * Rdata[0];
		  Gy[1] = ONE_LP_B*Gy[0] + ONE_LP_A * Rdata[1];
		  gz_biasdata[index] = ONE_LP_B*gz_biasdata[index-1] + ONE_LP_A*Rdata[2];
		 
		  ax_sum += Ax[1];
		  ay_sum += Ay[1];
		  az_sum += Az[1];
		  gx_sum += Gx[1];
		  gy_sum += Gy[1];
		  gz_sum += gz_biasdata[index];
			
			Ax[0] = Ax[1];
		  Ay[0] = Ay[1];
		  Az[0] = Az[1];
		  Gx[0] = Gx[1];
			Gy[0] = Gy[1];
	 }
	 acc_index = 0;
	 
	 Gz[0] = gz_biasdata[index-1];
	 
	 ax_bias = ax_sum / MAX_NUM;
	 ay_bias = ay_sum / MAX_NUM;
	 az_bias = az_sum / MAX_NUM;
	 
	 gx_bias = gx_sum / MAX_NUM;
	 gy_bias = gy_sum / MAX_NUM;
	 gz_bias = gz_sum / MAX_NUM;
	 
}

/*
从MPU9150得到数据
经过简单滤波后
进行欧拉角以及四元数的计算
*/
double MPU6050_GetData(double gyro[], double acc[])
{
	double temp_bias;
	static int count = 0;
	int flag = -1,i;
	
	while(flag)
	     flag = mpu_get_accel_reg(Rdata,0);
	Ax[1] = ONE_LP_B*Ax[0] + ONE_LP_A*Rdata[0];
	Ay[1] = ONE_LP_B*Ay[0] + ONE_LP_A*Rdata[1];
	Az[1] = ONE_LP_B*Az[0] + ONE_LP_A*Rdata[2];
	
	flag = -1;
	while(flag)
	     flag = mpu_get_gyro_reg(Rdata,0);
	Gx[1] = ONE_LP_B*Gx[0] + ONE_LP_A * Rdata[0];
	Gy[1] = ONE_LP_B*Gy[0] + ONE_LP_A * Rdata[1];
	Gz[1] = ONE_LP_B*Gz[0] + ONE_LP_A * Rdata[2];
	
	//kalman 平滑
	//Gz[1] = kalman_filter(&kalman,Gz[1]);   //kalman
	
	Ax[0]=Ax[1];
	Ay[0]=Ay[1];
	Az[0]=Az[1];
	Gx[0]=Gx[1];
	Gy[0]=Gy[1];
	Gz[0]=Gz[1];

	/*
	陀螺仪的滑动均值滤波 如果有磁力计的话，可以去掉. 
	小于THRO认为是稳定的，或者小于2*THRO但是编码器没动也认为是稳定的。
	加入编码器主要是为了减少零漂
	*/
	temp_bias = My_Abs(Gz[1] - gz_bias);
 	if( temp_bias < THRO )//|| ((temp_bias < 2*THRO) && (Odom_stable == 1)) )  //updata the gz_bias
 	{
 		gz_sum -= gz_biasdata[sample_cnt];
 		gz_biasdata[sample_cnt] = Gz[1];
 		gz_sum += Gz[1];
 		gz_bias = gz_sum / MAX_NUM;
 		if(++sample_cnt >= MAX_NUM)
 			 sample_cnt = 0;
 		gz_rate = 0;
 	}
 	else
	{
		gz_rate = (Gz[1] - gz_bias) / 16.4;
	}
	
	gx_rate = (Gx[1] - gx_bias) / 16.4;
	gy_rate = (Gy[1] - gy_bias) / 16.4;

	gx_rate = gx_rate / 57.29578;
	gy_rate = gy_rate / 57.29578;
	gz_rate = gz_rate / 57.29578;
	
	gyro[0] = gx_rate;
	gyro[1] = gy_rate;
	gyro[2] = gz_rate;
	
	/*加速度计的滑动均值滤波*/
	acc[0] = Ax[1];
	acc[1] = Ay[1];
	acc[2] = Az[1];
	/*加速度计的滤波影响了系统性能*/
//	for(i = 0; i<3 ;i++)
//	{
//		acc_sum[i] -= acc_array[i][acc_index];
//		acc_array[i][acc_index] = acc[i];
//		acc_sum[i] += acc[i];
//		acc[i] = acc_sum[i] / ACC_NUM;
//	}
//	if(++acc_index >= ACC_NUM)acc_index = 0;
	  return gz_rate;
}

char MPU6050_Check(void )
{
	uint8_t id;
	short flag = -1;
	int count = 1000;
	while(count--)
	{
		 flag = I2C_Read(0X68 ,0x75,1,&id);
		 if(id == 0x68 && flag == 0)
		 {
				return 1;
		 }
		 else flag = -1;
	}
	return 0;
}


void MPU6050_Init(void )
{
	IIC_Init();
	if(!MPU6050_Check())
	{
		while(1)
		{
//			OLED_Printf(0,0,"mpu6050_init_error");
//			Beep_On();
		}
	}
	mpu_init();  /*gyro=2000/s acc=4g rate=1khz bypass mode on */
	MPU6050_GetBias();
}




