#include "main.h"

S_Moto S_Moto_L,S_Moto_R;
void main(void)
{
    int i = 0;
    if ( SysTick_Config(SystemCoreClock/1000))
  { 
    while (1);
  }
	pid_init_struct();
	FLASH_Unlock();
	EE_INIT();
  KEY_Configuration();
	LED_Configuration();
	Encoder_Configuration();
	Moto_Init();
	BEEP_Configuration();
	USART1_Configuration();
	ADC1_Configuration();
	ADC3_Configuration();
 // BEEP_ON();
	
    delay_ms(500); //delay 500ms�� �ȴ�mpu6050�ϵ��ȶ�
    while(MPU6050_Initialization() == 0xff) 
    {
        i++;     //���һ�γ�ʼ��û�гɹ����Ǿ�����һ��                     
        if(i>10) //�����ʼ��һֱ���ɹ����Ǿ�ûϣ���ˣ�������ѭ����������һֱ��
        {
            while(1) 
            {
               LED_GREEN_TOGGLE();
                delay_ms(500);
            }
        }
    }
		 BEEP_OFF();
		offset_ad();
    //TIM6_Configuration();
    //TIM7_Configuration();
    MPU6050_Interrupt_Configuration();
	SystemInitReady=1;
    while(1)
    {
        if(mpu_6050_data_ok_flag)//��ȡ����������
        {
        MPU6050_ReadData();                                              
        MPU6050_Data_Filter();
        mpu_6050_data_ok_flag=0;
        }
		if(Attitude_500Hz)
	{   MPU6050_Angle_Calculate(MPU6050_Real_Data.Gyro_X* (3.14159265/180.0),//����ϳɽǶ�
        MPU6050_Real_Data.Gyro_Y* (3.14159265/180.0),
        MPU6050_Real_Data.Gyro_Z* (3.14159265/180.0),
        MPU6050_Real_Data.Accel_X,
        MPU6050_Real_Data.Accel_Y,
        MPU6050_Real_Data.Accel_Z);
		    get_irda_data();
        Attitude_500Hz=0;	
       // LDCRead();//��ȡldc1000��ֵ
        if(uart_mode)
        ANO_DT_Data_Exchange();
	}
    	if(Motor_100Hz)
	{
	
//            if(Go_Mode==SCAN)
//            Scan_Metal();//ɨ����˿
//            else if(Go_Mode==STOP)
//            {
//             Set_R_Motor_Voltage(0);
//             Set_L_Motor_Voltage(0);
//            }
           // 
		Motor_100Hz=0;
	}
       if(Encoder_50Hz)				
	{
 	if(ureal_irda_data1>3200&&ureal_irda_data2>3200)
	{
	Set_L_Motor_Voltage(0);
	Set_R_Motor_Voltage(0);
	Go_Mode=STOP;
	
	}
		Encoder_50Hz=0;
        
	}
		if(Com_10Hz)
		{
            uart_choise();
            calibrate_IMU();//У׼������
			Com_10Hz=0;
            if(uart_mode)
            LED_GREEN_TOGGLE();
            else
            {
            LED_GREEN_ON();
            BlueTooth_TX_Data.data1=(S_Moto_L.Encoder_V+S_Moto_R.Encoder_V)/2; 
            BlueTooth_TX_Data.data2=Go_Time; 
            BlueTooth_TX_Data.data3=(S_Moto_L.Encoder_L+S_Moto_R.Encoder_L)/2; 
            BlueTooth_TX_Data.data4=Expect_ProximityData-12000;
            BlueTooth_TX_Data.data5=ProximityData-12000;
            BlueTooth_TX_Data.data6=Set_Moto_Speed;    
            serial_Tx(&BlueTooth_TX_Data);//�������ݵ���λ��   
            }
          Serial_Key(&BlueToothkey);//������������      
		}
      if(Set_2Hz)
			{
       BEEP_OFF();//�ط�����
       Set_2Hz=0;
		 }			 
      
    }

}
