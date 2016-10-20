/****************************************************************
*��ת�������Ԫ����ʾ��
*
*			 q0^2+q1^2+q2^2+q3^2	2(q1q2 - q0q3)				2(q1q3+q0q2)
*Cbn = 2(q1q2+q0q3)					q0^2-q1^2+q2^2-q3^2	  2(q2q3-q0q1) 
*			 2(q1q3-q0q2)					2(q2q3+q0q1)					q0^2-q1^2-q2^2+q3^2
*��ת��������Ԫ������ʾ�Ļ�����Ψһ�ġ�����ת˳���޹أ�
*��Ϊ��Ԫ���ǵ�ЧΪһ����ת������������ת�ĵ���
*
*�����ŷ���Ǳ�ʾ����ô��ת���������ת˳���й��ˡ�
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

/**************************ʵ�ֺ���********************************************
*����ԭ��:void IMU_AHRSupdate
*��������:����AHRS ������Ԫ�� 
*�������:��ǰ�Ĳ���ֵ��
*�������:û��
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

  // �Ȱ���Щ�õõ���ֵ���
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
  
//  nowtime = GetSysTime();  //��ȡʱ��
  if(nowtime<lasttime){ //��ʱ��������ˡ�
  halfT =  ((double)(nowtime + (0xffff- lasttime)) / 2000000.0f);
  }
  else	{
  halfT =  ((double)(nowtime - lasttime) / 2000000.0f);
  }
  lasttime = nowtime;	//����ʱ��

//  norm = 1/sqrt(ax*ax + ay*ay + az*az);     
	norm = invSqrt(ax*ax + ay*ay + az*az);
  ax = ax * norm;
  ay = ay * norm;
  az = az * norm;
  //�ѼӼƵ���ά����ת�ɵ�λ������

//  norm = 1/sqrt(mx*mx + my*my + mz*mz);        
	norm = invSqrt(mx*mx + my*my + mz*mz);
  mx = mx * norm;
  my = my * norm;
  mz = mz * norm;

  // compute reference direction of flux
	/*
	hx hy hz���ѻ�������ϵ��õĴų�ǿ��ת�����ο�����ϵ����ֵ��
	(hx,hy,hz) = Cbn(mx,my,mz)
	*/
  hx = 2*mx*(0.5f - q2q2 - q3q3) + 2*my*(q1q2 - q0q3) + 2*mz*(q1q3 + q0q2);
  hy = 2*mx*(q1q2 + q0q3) + 2*my*(0.5f - q1q1 - q3q3) + 2*mz*(q2q3 - q0q1);
  hz = 2*mx*(q1q3 - q0q2) + 2*my*(q2q3 + q0q1) + 2*mz*(0.5f - q1q1 - q2q2);    
	
	/*
	�ο�����ϵ�¡�X��ָ�򱱷� Y��ָ�򶫷� Z��ָ���Ϸ�
	���ڵشŷ��� ������ ���Եش���Y����û�з����� 	���� by = 0
	ˮƽ����ֻ��X���з�������bx = sqrt(hx *��hx + hy*hy) bz = hz
	bx by bz ���ǲο�����ϵ��ʵ�ʵĵش�ǿ��
	*/
  bx = sqrt((hx*hx) + (hy*hy));
  bz = hz;     
  
  // estimated direction of gravity and flux (v and w)
	/*Vx y z��ʾ�Ѳο�����ϵ�µ�(0,0,g)ת������������ϵ�µ�ֵ
	*(Vx,Vy,Vz) = Cnb * (0,0,1) ��vxyz�����ݻ��ֺ����̬�����������������
	*/
  vx = 2*(q1q3 - q0q2);
  vy = 2*(q0q1 + q2q3);
  vz = q0q0 - q1q1 - q2q2 + q3q3;
	
	
	/*
	(wx,wy,wz) = Cnb * (bx,0,bz)
	�Ѳο�����ϵ�£�ʵ�ʵĵش�ǿ��ת������������ϵ�¡�
	wx wy wz Ϊʵ�ʵش�ǿ���ڻ�������ϵͳ����ֵ��
	*/
  wx = 2*bx*(0.5 - q2q2 - q3q3) + 2*bz*(q1q3 - q0q2);
  wy = 2*bx*(q1q2 - q0q3) + 2*bz*(q0q1 + q2q3);
  wz = 2*bx*(q0q2 + q1q3) + 2*bz*(0.5 - q1q1 - q2q2);  
  
  // error is sum of cross product between reference direction of fields and direction measured by sensors
	/*��λ�����������������ļнǾ���
	��λ�����Ĳ�˵��ڼнǵ�����ֵ = sin(a) ��a��С������� a=sin(a)
	����Ϊ������ һ���������ǻ��ֳ������������� �� ���������������֮������
								 һ���������ǻ��ֳ����Ĵų�ǿ�� �� ������Ĵų�ǿ��֮������
	*/						
  ex = (ay*vz - az*vy) + (my*wz - mz*wy);
  ey = (az*vx - ax*vz) + (mz*wx - mx*wz);
  ez = (ax*vy - ay*vx) + (mx*wy - my*wx);

  /*
  axyz�ǻ����������ϵ�ϣ����ٶȼƲ����������������Ҳ����ʵ�ʲ����������������
axyz�ǲ����õ�������������vxyz�����ݻ��ֺ����̬����������������������Ƕ��ǻ����������ϵ�ϵ�����������
������֮�������������������ݻ��ֺ����̬�ͼӼƲ��������̬֮�����
������������������������Ҳ�������������ˣ�����ʾ��exyz�����������������Ĳ����
�����������Ծ���λ�ڻ�������ϵ�ϵģ������ݻ������Ҳ���ڻ�������ϵ�����Ҳ���Ĵ�С�����ݻ����������ȣ����������������ݡ�
��������Լ��ö�������һ�£����������ǶԻ���ֱ�ӻ��֣����Զ����ݵľ�������ֱ�������ڶԻ�������ϵ�ľ�����
  */
	
	
/*
	��һ�δ��벻�Ǻܶ� Ϊʲô������PI����
	1.14 ����һƪ���� ���ˡ�
	������ʵ��������ΪR 
	�����ں����ΪRR
	�����Ƿ���Ϊ R0 = R + Ul						UlΪ��Ƶ�ۼ����
	���ٶȺʹ����Ʒ���Ϊ Ra = R + Uh		UhΪ��Ƶ�����
	����һ��ȫͨ�˲��� C(s)
	�� Gl(s) = C/(s+C)  Gh(s) = s/(s+C) GlΪ��ͨ�˲��� GhΪ��ͨ�˲���
	RR = R0*Gh + Ra*Gl = R + Ul*Gh + Uh*Gl = R(��Ƶ�ź�ͨ����Ƶ�˲��� ��Ƶ�ź�ͨ����Ƶ�˲��� �������˵� �˵Ⱥ���Լ����)
	��ʽ�ĳ���˵�������˲�����Ч���ںϡ��˳��˴˵�������
	�������ҵ�΢�ַ��� R' = R*Delta			��DeltaΪ�����ʵķ��Գɾ���
	����һ����ʽ�任 sRa(s) = RR(s)*Delta(s)
	Ra(s) = ((s+C)*RR - Ra) / s
	������ʽ�ɵ�
	sRR(s) = C(s)*(Ra(s) - RR(S)) + RR(s)*Delta(s)
	������ʽ���任��
	RR' = C*(Ra - RR) + RR*Delta
	ֻҪCΪȫͨ�˲��������㡣
	����CΪ����KP ����P������
	����Ϊ���ֻ��ڿ���������̬���
	�������Ǽ���C(s) = KP + KI/s��
	���Կ��������������PI�������������
	KPȷ����ֹƵ��f = KP/(2*PI) KIȷ��������̬����ʱ��
	һ����˵ KI = (0.01,0.1)*KP
	PS �����ƺͼ��ٶȼƵ��������ۼӵ�ԭ����:
	������ֻ����Yaw ���ٶȼ�ֻ����Roll Pitch �����໥����������Ӱ�� ���Կ����ۼ� 
*/
if(ex != 0.0f && ey != 0.0f && ez != 0.0f){
  exInt = exInt + ex * Ki * halfT;
  eyInt = eyInt + ey * Ki * halfT;	
  ezInt = ezInt + ez * Ki * halfT;

  // �ò���������PI����������ƫ
  gx = gx + Kp*ex + exInt;
  gy = gy + Kp*ey + eyInt;
  gz = gz + Kp*ez + ezInt;

  }

  /**/
  /*
  ��Ԫ��΢�ַ��̵�Ƥ����ⷨ����˵
	��Ч��תʸ���������㷨�Ľ���cos(a/2) = 1 sin(a/2) = a/2
	1   -gx   -gy   -gz
	gx   1     gz   -gy
	gy  -gz    1     gx
	gz   gy    -gx   1
  �������㷨����Բ׶�˶��˶���ʱ����бȽϴ�����
	������Ըĳɵ�Ч��תʸ����˫���������������㷨
  */
  tempq0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
  tempq1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
  tempq2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
  tempq3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;  
  
  // ��Ԫ���淶��
  norm = invSqrt(tempq0*tempq0 + tempq1*tempq1 + tempq2*tempq2 + tempq3*tempq3);
  q0 = tempq0 * norm;
  q1 = tempq1 * norm;
  q2 = tempq2 * norm;
  q3 = tempq3 * norm;
	
//	Yaw =	atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2 * q2 - 2 * q3 * q3 + 1) ;	
//	Pitch  = asin(2 * q0 * q2 - 2 * q1 * q3);
//	Roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1);
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:void IMU_GetQ(double *q)
*��������:�õ���Ԫ��
*�������:ָ����Ԫ����ָ��
*�������:��
*******************************************************************************/
void IMU_GetQ(double *q)
{
	q[0] = q0;
	q[1] = q1;
	q[2] = q2;
	q[3] = q3;
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:void IMU_GetEuler(double *yaw,double *pitch,double *roll)
*��������:����Ԫ��ת��Ϊŷ����
*�������:ָ��ŷ���ǵ�ָ��
*�������:��
*******************************************************************************/
void IMU_GetEuler(double *yaw,double *pitch,double *roll)
{
	/*
	��Ԫ����ŷ���ǵ�ת����
	���ڱ�������ת˳��ΪZYX
	Pitch = arcsin(-T31)
	Roll  = arctan(T32/T33)
	Yaw   = arctan(T21/T11)
	Txx���Ƿ������Ҿ���Cbn�е�Ԫ��
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


/**************************ʵ�ֺ���********************************************
*����ԭ��:void IMU_Update(double gx,double gy,double gz, 
*													double ax,double ay,double az,
*													double mx,double my,double mz)
*��������:����������ٶȼƵ��ں�
*�������:����������ֵ
*�������:��
*******************************************************************************/
void IMU_Update(double gx,double gy,double gz, double ax,double ay,double az)
{
	  double norm,halfT;
		double vx, vy, vz;
		double ex, ey, ez;
		static double exInt = 0, eyInt = 0, ezInt = 0;  // scaled integral error
	  static double nowtime = 0,lasttime = 0;
    
	  //�õ�ϵͳʱ�䣬����ϵͳʱ��(us)ת��Ϊs
		nowtime = GetSystemTime();
		halfT = (double)(nowtime - lasttime) / 2000000;
		lasttime = nowtime;
	
	  //normalise the measurements
		//���ٶȼƲ�õĻ�������ϵ�����������������
    norm = 1/sqrt(ax*ax + ay*ay + az*az);	
	  ax = ax * norm;
	  ay = ay * norm;
	  az = az * norm;
		
	  //estimated direction of gravity
		//�������ǻ��ֵó��Ļ�������ϵ�����������������:�������Ҿ���*[0 0 1]'
	  vx = 2*(q1*q3 - q0*q2);
		vy = 2*(q0*q1 + q2*q3);
		vz = q0*q0 - q1*q1 - q2*q2 + q3*q3;
	   
	  // error is sum of cross product between reference direction of fields and direction measured by sensors
		//�����������������
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
		gz = gz + Kp*ez;// + ezInt; ���ٶȼ�û��У��Z�ᣬ����û��Ҫ����
		
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


/*��Ԫ����ʼ��*/
/**************************ʵ�ֺ���********************************************
*����ԭ��:void IMU_Init(void )
*��������:��Ԫ���ĳ�ʼ�� 
*					���ݼ��ٶȼƺ������ǵ������������ŷ���� Ȼ�������Ԫ��
*�������:��
*�������:û��
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
		
	/*�����ƹ�һ��*/
	
	
	//��ʼ��ŷ���ǣ���λ������
	//������ֶ���ϵ��X��Ϊǰ��������X����תΪ�����ROLL����Y����ת�Ǹ�����PITCH����Z����ת��ƫ����YAW
	/*
	��(ax,ay,az) = Cnb*(0,0,1) ���Եõ� Roll �� Pitch�ı��ʽ��
	ax = -sin(Pitch)
	ay = cos(Pitch)sin(Roll)
	az = cos(Pitch)cos(Roll)
	==> Pitch = -arcsin(ax)  Roll = atan2(ay,az)
	*/ 
  Roll  =  atan2(ay, az);
  Pitch = - asin(ax);
	Yaw = 0;
	
	/*
	������ˮƽ��ʱ��
	tan(yaw)=my/mx��
	��������ˮƽ��ʱ����Ҫ�Ѵ��������������ϵ�ֵ�ֱ�ͶӰ��ƽ��x y���ϡ�
	(xh,yh,zh) = Cbn*(mx,my,mz)
	���ڲ���Ҫ��Z����ת ����Tz = E. => Cnb = Tx*Ty*E = Tx*Ty =>Cbn =Cnb^T = Ty^T * Tx^T
	�ɴ˿������xh,yh�ı��ʽ
	xh = mx*cos(Pitch) + mz*cos(Roll)*sin(Pitch) + my*sin(Pitch)*sin(Roll)
	yh = my*cos(Roll) - mz*sin(Roll)
	Yaw = atan2(yh,xh)
	*/
//	yh = my * cos(Roll) - mz * sin(Roll);
//	xh = mx*cos(Pitch) + my*sin(Roll)*sin(Pitch) + mz * cos(Roll)*sin(Pitch);
//	Yaw = atan2(yh,xh);
	
//	USART_Printf(USART2,"init:%.2f %.2f %.2f A:%.2f %.2f %.2f\n",mx,my,mz,Pitch*57.3,Roll*57.3,Yaw*57.3);
  /*MWC�ɿش���*/
	
	/*
	//��ʼ����Ԫ������ת˳��ZYX
	��ŷ����ֱ�ӱ�ʾ��Ԫ��
	
	ŷ���Ǳ�ʾ����������������ת�ĵ��ӡ�
	QZ = (cos(0.5*Yaw),sin(0.5*Yaw)*(0,0,1))			Z����ת	
	Qy = (cos(0.5*Pitch),sin(0.5*Pitch)*(0,1,0))	Y����ת
	Qx = (cos(0.5*Roll),sin(0.5*Roll)*(1,0,0))		X����ת
	
	��Ԫ����ʾһ����ת������һ����תΪQ=(q0,q1,q2,q3)
	��ô
	Q = (q0,q1,q2,q3) = QX*QY*QZ	=M(QX)M(QY)QZ	*Ϊ��Ԫ���˷�
	
	MQZ=[QZ(1) -QZ(2) -QZ(3) -QZ(4);QZ(2) QZ(1) -QZ(4) QZ(3);QZ(3) QZ(4) QZ(1) -QZ(2);QZ(4) -QZ(3) QZ(2) QZ(1)]
	MQX MQY����
	��ӦԪ����� ���ɵõ������ʽ��
	*/
  q0 = cos(0.5f*Roll)*cos(0.5f*Pitch)*cos(0.5f*Yaw) - sin(0.5f*Roll)*sin(0.5f*Pitch)*sin(0.5f*Yaw); 
  q1 = sin(0.5f*Roll)*cos(0.5f*Pitch)*cos(0.5f*Yaw) + cos(0.5f*Roll)*sin(0.5f*Pitch)*sin(0.5f*Yaw); 
  q2 = cos(0.5f*Roll)*sin(0.5f*Pitch)*cos(0.5f*Yaw) - sin(0.5f*Roll)*cos(0.5f*Pitch)*sin(0.5f*Yaw);  
  q3 = cos(0.5f*Roll)*cos(0.5f*Pitch)*sin(0.5f*Yaw) + sin(0.5f*Roll)*sin(0.5f*Pitch)*cos(0.5f*Yaw); 
	
	/*��һ��*/
	norm = 1/sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
	q0 = q0 * norm;
	q1 = q1 * norm;
	q2 = q2 * norm;
	q3 = q3 * norm;
	
}

/**************************ʵ�ֺ���********************************************
*����ԭ��:IMU_Cal(void )
*��������:��ȡ��������Ϣ�͵�����̬���㺯����������Ԫ��
*�������:��
*�������:û��
*******************************************************************************/
double IMU_Cal(void )
{
	double gyro[3],acc[3];
	
	MPU6050_GetData(gyro,acc);
//	IMU_AHRSupdate(gyro[0],gyro[1],gyro[2],acc[0],acc[1],acc[2],mag[0],mag[1],mag[2]);
	IMU_Update(gyro[0],gyro[1],gyro[2],acc[0],acc[1],acc[2]);
	
	return gyro[2];
}


