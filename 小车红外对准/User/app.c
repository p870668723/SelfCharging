#include "main.h"

u8 calibrate_Ldc1000_flag,uart_mode=1;
float Expect_ProximityData,Set_Moto_Speed;
u32 Scan_ProximityData_Max=0,Scan_ProximityData_Min=0xffffffff;

#define Scan_Speed 1600
/******************************************************************************
 * 函数名称: Scan_Metal
 * 函数功能: 扫描铁丝，得到循迹的期望位置
 * 入口参数: 无
 ******************************************************************************/
void Scan_Metal(void)
{

        static u16 time=0;
        if(Scan_ProximityData_Max<ProximityData)
        Scan_ProximityData_Max=ProximityData;
        if(Scan_ProximityData_Min>ProximityData)
        Scan_ProximityData_Min=ProximityData;
        time++;
        if(time<50)
        {
        Set_L_Motor_Voltage(Scan_Speed);
        Set_R_Motor_Voltage(-Scan_Speed);
        }
        else if(time>=50&&time<100)
        {
        Set_L_Motor_Voltage(-Scan_Speed);
        Set_R_Motor_Voltage(Scan_Speed);
        }

        else
        {
        Set_L_Motor_Voltage(0);
        Set_R_Motor_Voltage(0);
        Go_Mode=STOP;
        time=0;
        Expect_ProximityData=(Scan_ProximityData_Max-Scan_ProximityData_Min)/3+Scan_ProximityData_Min;   
        }
   
}

/******************************************************************************
 * 函数名称: Moto_Ctrl
 * 函数功能: 根据循迹的期望位置pid调节电机输出
 * 入口参数: 无
 ******************************************************************************/
void Moto_Ctrl(void)
{
    static float PID_LDC1000_Out,PID_Gyro_Yaw_Out;
    static s16 Moto_Out_L,Moto_Out_R;
	
    PID_LDC1000_Out=PID_Calculate(&PID_LDC1000,sreal_irda_Position ,0);//pid调节位置
    PID_Gyro_Yaw_Out=PID_Calculate(&PID_Gyro_Yaw_Struct,MPU6050_Real_Data.Gyro_Z, PID_LDC1000_Out);//pid调节角速度
    Moto_Out_L=Set_Moto_Speed-PID_Gyro_Yaw_Out;
    Moto_Out_R=Set_Moto_Speed+PID_Gyro_Yaw_Out;
    Set_L_Motor_Voltage(Moto_Out_L);//左电机输出
    Set_R_Motor_Voltage(Moto_Out_R);//右电机输出
}
void uart_choise(void)//调试模式选择
{
    static u16 num1,num2;
    if(Key1==0)
    {
       num1++;
       if(num1>10)
       {
        uart_mode=1;
           return;
       } 
       uart_mode=0; 
    }
    else
    {
       num1=0;
    }

    if(Key0==0) 
    {
        
        num2++;
        if(num2>10)
        {
        Go_Mode=GO; 
        return;    
        }

       Go_Mode=SCAN;        
    }
    else
        num2=0;
     
}
