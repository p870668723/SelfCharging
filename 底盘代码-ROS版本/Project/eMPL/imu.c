/****************************************************************
*旋转矩阵的四元数表示法
*
*			 q0^2+q1^2+q2^2+q3^2	2(q1q2 - q0q3)				2(q1q3+q0q2)
*Cbn = 2(q1q2+q0q3)					q0^2-q1^2+q2^2-q3^2	  2(q2q3-q0q1) 
*			 2(q1q3-q0q2)					2(q2q3+q0q1)					q0^2-q1^2-q2^2+q3^2
*旋转矩阵用四元数来表示的话就是唯一的。与旋转顺序无关，
*因为四元数是等效为一次旋转而不是三次旋转的叠加
*
*如果用欧拉角表示，那么旋转矩阵就与旋转顺序有关了。
*
*Cnb = Cbn^T
******************************************************************/
#include "imu.h"
#include "oled.h"
#include "timer.h"
#include "mymath.h"

#define USARTX        USART1

#define PI            3.1415926
double q0 = 1,q1 = 0,q2 = 0,q3 = 0;
double Yaw,Pitch,Roll;
double MAG_YAW;

/**************************实现函数********************************************
*函数原型:void IMU_AHRSupdate
*功　　能:更新AHRS 更新四元数 
*输入参数:当前的测量值。
*输出参数:没有
*******************************************************************************/
#define Kp 2.0f   // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 0.005f   // integral gain governs rate of convergence of gyroscope biases

void IMU_AHRSupdate(double gx, double gy, double gz, double ax, double ay, double az, double mx, double my, double mz) {
  double norm;
  double hx, hy, hz, bx, bz;
  double vx, vy, vz, wx, wy, wz;
  double ex, ey, ez,halfT;
  double tempq0,tempq1,tempq2,tempq3;
	double static exInt = 0,eyInt = 0,ezInt = 0;
	static double nowtime,lasttime;

  // 先把这些用得到的值算好
  double q0q0 = q0*q0;
  double q0q1 = q0*q1;
  double q0q2 = q0*q2;
  double q0q3 = q0*q3;
  double q1q1 = q1*q1;
  double q1q2 = q1*q2;
  double q1q3 = q1*q3;
  double q2q2 = q2*q2;   
  double q2q3 = q2*q3;
  double q3q3 = q3*q3;          
  
//  nowtime = GetSysTime();  //读取时间
  if(nowtime<lasttime){ //定时器溢出过了。
  halfT =  ((double)(nowtime + (0xffff- lasttime)) / 2000000.0f);
  }
  else	{
  halfT =  ((double)(nowtime - lasttime) / 2000000.0f);
  }
  lasttime = nowtime;	//更新时间

//  norm = 1/sqrt(ax*ax + ay*ay + az*az);     
	norm = invSqrt(ax*ax + ay*ay + az*az);
  ax = ax * norm;
  ay = ay * norm;
  az = az * norm;
  //把加计的三维向量转成单位向量。

//  norm = 1/sqrt(mx*mx + my*my + mz*mz);        
	norm = invSqrt(mx*mx + my*my + mz*mz);
  mx = mx * norm;
  my = my * norm;
  mz = mz * norm;

  // compute reference direction of flux
	/*
	hx hy hz即把机体坐标系测得的磁场强度转换到参考坐标系的数值。
	(hx,hy,hz) = Cbn(mx,my,mz)
	*/
  hx = 2*mx*(0.5f - q2q2 - q3q3) + 2*my*(q1q2 - q0q3) + 2*mz*(q1q3 + q0q2);
  hy = 2*mx*(q1q2 + q0q3) + 2*my*(0.5f - q1q1 - q3q3) + 2*mz*(q2q3 - q0q1);
  hz = 2*mx*(q1q3 - q0q2) + 2*my*(q2q3 + q0q1) + 2*mz*(0.5f - q1q1 - q2q2);    
	
	/*
	参考坐标系下。X轴指向北方 Y轴指向东方 Z轴指向上方
	由于地磁方向 由南向北 所以地磁在Y轴是没有分量的 	所以 by = 0
	水平方向只有X轴有分量。即bx = sqrt(hx *　hx + hy*hy) bz = hz
	bx by bz 就是参考坐标系下实际的地磁强度
	*/
  bx = sqrt((hx*hx) + (hy*hy));
  bz = hz;     
  
  // estimated direction of gravity and flux (v and w)
	/*Vx y z表示把参考坐标系下的(0,0,g)转换到机体坐标系下的值
	*(Vx,Vy,Vz) = Cnb * (0,0,1) 即vxyz是陀螺积分后的姿态来推算出的重力向量
	*/
  vx = 2*(q1q3 - q0q2);
  vy = 2*(q0q1 + q2q3);
  vz = q0q0 - q1q1 - q2q2 + q3q3;
	
	
	/*
	(wx,wy,wz) = Cnb * (bx,0,bz)
	把参考坐标系下，实际的地磁强度转换到机体坐标系下。
	wx wy wz 为实际地磁强度在机体坐标系统的数值。
	*/
  wx = 2*bx*(0.5 - q2q2 - q3q3) + 2*bz*(q1q3 - q0q2);
  wy = 2*bx*(q1q2 - q0q3) + 2*bz*(q0q1 + q2q3);
  wz = 2*bx*(q0q2 + q1q3) + 2*bz*(0.5 - q1q1 - q2q2);  
  
  // error is sum of cross product between reference direction of fields and direction measured by sensors
	/*单位向量的误差由向量间的夹角决定
	单位向量的叉乘等于夹角的正弦值 = sin(a) 在a很小的情况下 a=sin(a)
	误差分为两部分 一个是陀螺仪积分出来的重力向量 和 测出来的重力向量之间的误差
								 一个是陀螺仪积分出来的磁场强度 和 测出来的磁场强度之间的误差
	*/						
  ex = (ay*vz - az*vy) + (my*wz - mz*wy);
  ey = (az*vx - ax*vz) + (mz*wx - mx*wz);
  ez = (ax*vy - ay*vx) + (mx*wy - my*wx);

  /*
  axyz是机体坐标参照系上，加速度计测出来的重力向量，也就是实际测出来的重力向量。
axyz是测量得到的重力向量，vxyz是陀螺积分后的姿态来推算出的重力向量，它们都是机体坐标参照系上的重力向量。
那它们之间的误差向量，就是陀螺积分后的姿态和加计测出来的姿态之间的误差。
向量间的误差，可以用向量叉积（也叫向量外积、叉乘）来表示，exyz就是两个重力向量的叉积。
这个叉积向量仍旧是位于机体坐标系上的，而陀螺积分误差也是在机体坐标系，而且叉积的大小与陀螺积分误差成正比，正好拿来纠正陀螺。
（你可以自己拿东西想象一下）由于陀螺是对机体直接积分，所以对陀螺的纠正量会直接体现在对机体坐标系的纠正。
  */
	
	
/*
	这一段代码不是很懂 为什么可以做PI修正
	1.14 看了一篇论文 懂了。
	假设真实方向余弦为R 
	互补融合输出为RR
	陀螺仪方向为 R0 = R + Ul						Ul为低频累计误差
	加速度和磁力计方向为 Ra = R + Uh		Uh为高频振动误差
	假设一个全通滤波器 C(s)
	则 Gl(s) = C/(s+C)  Gh(s) = s/(s+C) Gl为低通滤波器 Gh为高通滤波器
	RR = R0*Gh + Ra*Gl = R + Ul*Gh + Uh*Gl = R(低频信号通过高频滤波器 高频信号通过低频滤波器 都被过滤掉 此等号是约等于)
	上式的成立说明互补滤波能有效的融合、滤除彼此的噪声。
	方向余弦的微分方程 R' = R*Delta			（Delta为角速率的反对成矩阵）
	进行一次拉式变换 sRa(s) = RR(s)*Delta(s)
	Ra(s) = ((s+C)*RR - Ra) / s
	代入上式可得
	sRR(s) = C(s)*(Ra(s) - RR(S)) + RR(s)*Delta(s)
	进行拉式反变换得
	RR' = C*(Ra - RR) + RR*Delta
	只要C为全通滤波器都满足。
	假设C为常数KP 则是P修正。
	又因为积分环节可以消除稳态误差
	所以我们假设C(s) = KP + KI/s。
	所以可以用误差来进行PI修正来消除误差
	KP确定截止频率f = KP/(2*PI) KI确定修正静态误差的时间
	一般来说 KI = (0.01,0.1)*KP
	PS 磁力计和加速度计的误差可以累加的原因是:
	磁力计只修正Yaw 加速度计只修正Roll Pitch 他们相互独立、互不影响 所以可以累加 
*/
if(ex != 0.0f && ey != 0.0f && ez != 0.0f){
  exInt = exInt + ex * Ki * halfT;
  eyInt = eyInt + ey * Ki * halfT;	
  ezInt = ezInt + ez * Ki * halfT;

  // 用叉积误差来做PI修正陀螺零偏
  gx = gx + Kp*ex + exInt;
  gy = gy + Kp*ey + eyInt;
  gz = gz + Kp*ez + ezInt;

  }

  /**/
  /*
  四元数微分方程的皮卡求解法或者说
	等效旋转矢量单子样算法的近似cos(a/2) = 1 sin(a/2) = a/2
	1   -gx   -gy   -gz
	gx   1     gz   -gy
	gy  -gz    1     gx
	gz   gy    -gx   1
  单子样算法在做圆锥运动运动的时候会有比较大的误差
	后面可以改成等效旋转矢量的双子样或者三子样算法
  */
  tempq0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
  tempq1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
  tempq2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
  tempq3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;  
  
  // 四元数规范化
  norm = invSqrt(tempq0*tempq0 + tempq1*tempq1 + tempq2*tempq2 + tempq3*tempq3);
  q0 = tempq0 * norm;
  q1 = tempq1 * norm;
  q2 = tempq2 * norm;
  q3 = tempq3 * norm;
	
//	Yaw =	atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2 * q2 - 2 * q3 * q3 + 1) ;	
//	Pitch  = asin(2 * q0 * q2 - 2 * q1 * q3);
//	Roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1);
}

/**************************实现函数********************************************
*函数原型:void IMU_GetQ(double *q)
*功　　能:得到四元数
*输入参数:指向四元数的指针
*输出参数:无
*******************************************************************************/
void IMU_GetQ(double *q)
{
	q[0] = q0;
	q[1] = q1;
	q[2] = q2;
	q[3] = q3;
}

/**************************实现函数********************************************
*函数原型:void IMU_GetEuler(double *yaw,double *pitch,double *roll)
*功　　能:把四元数转换为欧拉角
*输入参数:指向欧拉角的指针
*输出参数:无
*******************************************************************************/
void IMU_GetEuler(double *yaw,double *pitch,double *roll)
{
	/*
	四元数与欧拉角的转换。
	由于本程序旋转顺序为ZYX
	Pitch = arcsin(-T31)
	Roll  = arctan(T32/T33)
	Yaw   = arctan(T21/T11)
	Txx都是方向余弦矩阵Cbn中的元素
	*/
	*yaw =	atan2(2 * (q1 * q2 + q0 * q3), -2 * q2 * q2 - 2 * q3 * q3 + 1) * 57.3f;	
	*pitch  = asin(2 * (q0 * q2 - q1 * q3)) * 57.3f;
	*roll = atan2(2 * (q2 * q3 + q0 * q1), -2 * q1 * q1 - 2 * q2* q2 + 1) * 57.3f;
}

double IMU_GetYawRadian(void )
{
	double yaw;
	yaw =	atan2(2 * (q1 * q2 + q0 * q3), -2 * q2 * q2 - 2 * q3 * q3 + 1);
  return yaw;	
}

double IMU_GetYawDegree(void )
{
	double yaw;
	yaw =	atan2(2 * (q1 * q2 + q0 * q3), -2 * q2 * q2 - 2 * q3 * q3 + 1) * 57.295f;
	return yaw;
}


/**************************实现函数********************************************
*函数原型:void IMU_Update(double gx,double gy,double gz, 
*													double ax,double ay,double az,
*													double mx,double my,double mz)
*功　　能:陀螺仪与加速度计的融合
*输入参数:各传感器数值
*输出参数:无
*******************************************************************************/
void IMU_Update(double gx,double gy,double gz, double ax,double ay,double az)
{
	  double norm,halfT;
		double vx, vy, vz;
		double ex, ey, ez;
		static double exInt = 0, eyInt = 0, ezInt = 0;  // scaled integral error
	  static double nowtime = 0,lasttime = 0;
    
	  //得到系统时间，并从系统时间(us)转换为s
		nowtime = GetSystemTime();
		halfT = (double)(nowtime - lasttime) / 2000000;
		lasttime = nowtime;
	
	  //normalise the measurements
		//加速度计测得的机体坐标系上三个轴的重力分量
    norm = 1/sqrt(ax*ax + ay*ay + az*az);	
	  ax = ax * norm;
	  ay = ay * norm;
	  az = az * norm;
		
	  //estimated direction of gravity
		//由陀螺仪积分得出的机体坐标系上三个轴的重力分量:方向余弦矩阵*[0 0 1]'
	  vx = 2*(q1*q3 - q0*q2);
		vy = 2*(q0*q1 + q2*q3);
		vz = q0*q0 - q1*q1 - q2*q2 + q3*q3;
	   
	  // error is sum of cross product between reference direction of fields and direction measured by sensors
		//两个重力向量作叉乘
		ex = (ay*vz - az*vy);
		ey = (az*vx - ax*vz);
		ez = (ax*vy - ay*vx);
	  
		// integral error scaled integral gain
		exInt = exInt + ex*Ki;
		eyInt = eyInt + ey*Ki;
		ezInt = ezInt + ez*Ki;
		
		// adjusted gyroscope measurements
		gx = gx + Kp*ex + exInt;
		gy = gy + Kp*ey + eyInt;
		gz = gz + Kp*ez;// + ezInt; 加速度计没法校正Z轴，所以没必要积分
		
		q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
		q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
		q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
		q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;  
		
		// normalise quaternion
		norm = 1/sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
		q0 = q0 * norm;
		q1 = q1 * norm;       
		q2 = q2 * norm;    
		q3 = q3 * norm;
}


/*四元数初始化*/
/**************************实现函数********************************************
*函数原型:void IMU_Init(void )
*功　　能:四元数的初始化 
*					根据加速度计和陀螺仪的数据来计算出欧拉角 然后计算四元数
*输入参数:无
*输出参数:没有
*******************************************************************************/
void IMU_Init(void )
{
	double norm,ax,ay,az,mx,my,mz,xh,yh;
	
	ax = ax_bias;
	ay = ay_bias;
	az = az_bias;
	
	norm = 1/sqrt(ax*ax+ay*ay+az*az);
	ax *= norm;
	ay *= norm;
	az *= norm;
		
	/*磁力计归一化*/
	
	
	//初始化欧拉角，单位：弧度
	//遵从右手定则建系，X轴为前进方向，绕X轴旋转为横滚角ROLL，绕Y轴旋转是俯仰角PITCH，绕Z轴旋转是偏航角YAW
	/*
	由(ax,ay,az) = Cnb*(0,0,1) 可以得到 Roll 和 Pitch的表达式。
	ax = -sin(Pitch)
	ay = cos(Pitch)sin(Roll)
	az = cos(Pitch)cos(Roll)
	==> Pitch = -arcsin(ax)  Roll = atan2(ay,az)
	*/ 
  Roll  =  atan2(ay, az);
  Pitch = - asin(ax);
	Yaw = 0;
	
	/*
	当处于水平的时候
	tan(yaw)=my/mx。
	当不处于水平的时候，需要把磁力计在三个轴上的值分别投影到平面x y轴上。
	(xh,yh,zh) = Cbn*(mx,my,mz)
	由于不需要绕Z轴旋转 所以Tz = E. => Cnb = Tx*Ty*E = Tx*Ty =>Cbn =Cnb^T = Ty^T * Tx^T
	由此可以求得xh,yh的表达式
	xh = mx*cos(Pitch) + mz*cos(Roll)*sin(Pitch) + my*sin(Pitch)*sin(Roll)
	yh = my*cos(Roll) - mz*sin(Roll)
	Yaw = atan2(yh,xh)
	*/
//	yh = my * cos(Roll) - mz * sin(Roll);
//	xh = mx*cos(Pitch) + my*sin(Roll)*sin(Pitch) + mz * cos(Roll)*sin(Pitch);
//	Yaw = atan2(yh,xh);
	
//	USART_Printf(USART2,"init:%.2f %.2f %.2f A:%.2f %.2f %.2f\n",mx,my,mz,Pitch*57.3,Roll*57.3,Yaw*57.3);
  /*MWC飞控代码*/
	
	/*
	//初始化四元数，旋转顺序：ZYX
	用欧拉角直接表示四元数
	
	欧拉角表示法，代表着三次旋转的叠加。
	QZ = (cos(0.5*Yaw),sin(0.5*Yaw)*(0,0,1))			Z轴旋转	
	Qy = (cos(0.5*Pitch),sin(0.5*Pitch)*(0,1,0))	Y轴旋转
	Qx = (cos(0.5*Roll),sin(0.5*Roll)*(1,0,0))		X轴旋转
	
	四元数表示一次旋转，假设一次旋转为Q=(q0,q1,q2,q3)
	那么
	Q = (q0,q1,q2,q3) = QX*QY*QZ	=M(QX)M(QY)QZ	*为四元数乘法
	
	MQZ=[QZ(1) -QZ(2) -QZ(3) -QZ(4);QZ(2) QZ(1) -QZ(4) QZ(3);QZ(3) QZ(4) QZ(1) -QZ(2);QZ(4) -QZ(3) QZ(2) QZ(1)]
	MQX MQY类似
	对应元素相等 即可得到下面的式子
	*/
  q0 = cos(0.5f*Roll)*cos(0.5f*Pitch)*cos(0.5f*Yaw) - sin(0.5f*Roll)*sin(0.5f*Pitch)*sin(0.5f*Yaw); 
  q1 = sin(0.5f*Roll)*cos(0.5f*Pitch)*cos(0.5f*Yaw) + cos(0.5f*Roll)*sin(0.5f*Pitch)*sin(0.5f*Yaw); 
  q2 = cos(0.5f*Roll)*sin(0.5f*Pitch)*cos(0.5f*Yaw) - sin(0.5f*Roll)*cos(0.5f*Pitch)*sin(0.5f*Yaw);  
  q3 = cos(0.5f*Roll)*cos(0.5f*Pitch)*sin(0.5f*Yaw) + sin(0.5f*Roll)*sin(0.5f*Pitch)*cos(0.5f*Yaw); 
	
	/*归一化*/
	norm = 1/sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
	q0 = q0 * norm;
	q1 = q1 * norm;
	q2 = q2 * norm;
	q3 = q3 * norm;
	
}

/**************************实现函数********************************************
*函数原型:IMU_Cal(void )
*功　　能:读取传感器信息和调用姿态结算函数来计算四元数
*输入参数:无
*输出参数:没有
*******************************************************************************/
double IMU_Cal(void )
{
	double gyro[3],acc[3];
	
	MPU6050_GetData(gyro,acc);
//	IMU_AHRSupdate(gyro[0],gyro[1],gyro[2],acc[0],acc[1],acc[2],mag[0],mag[1],mag[2]);
	IMU_Update(gyro[0],gyro[1],gyro[2],acc[0],acc[1],acc[2]);
	
	return gyro[2];
}


