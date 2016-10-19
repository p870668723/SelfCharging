#include "app.H"
#include "stmflash.H"
#include "mpu6050_driver.h"
#include "control.h"
#define EE_6050_ACC_X_OFFSET_ADDR	0
#define EE_6050_ACC_Y_OFFSET_ADDR	1
#define EE_6050_ACC_Z_OFFSET_ADDR	2
#define EE_6050_GYRO_X_OFFSET_ADDR	3
#define EE_6050_GYRO_Y_OFFSET_ADDR	4
#define EE_6050_GYRO_Z_OFFSET_ADDR	5
#define EE_PID_ROL_P	6
#define EE_PID_ROL_I	7
#define EE_PID_ROL_D	8
#define EE_PID_PIT_P	9

#define EE_PID_PIT_I	10
#define EE_PID_PIT_D	11
#define EE_PID_YAW_P	12
#define EE_PID_YAW_I	13
#define EE_PID_YAW_D	14
#define EE_PID_ALT_P	15
#define EE_PID_ALT_I	16
#define EE_PID_ALT_D	17
#define EE_PID_POS_P	18
#define EE_PID_POS_I	19
#define EE_PID_POS_D	20

#define EE_PID_PID1_P	21
#define EE_PID_PID1_I	22
#define EE_PID_PID1_D	23
#define EE_PID_PID2_P	24
#define EE_PID_PID2_I	25
#define EE_PID_PID2_D	26
#define EE_PID_PID3_P	27
#define EE_PID_PID3_I	28
#define EE_PID_PID3_D	29
#define EE_PID_PID4_P	30
#define EE_PID_PID4_I	31
#define EE_PID_PID4_D	32
#define EE_PID_PID5_P	33
#define EE_PID_PID5_I	34
#define EE_PID_PID5_D	35
#define EE_PID_PID6_P	36
#define EE_PID_PID6_I	37
#define EE_PID_PID6_D	38
#define EE_PID_PID7_P	39
#define EE_PID_PID7_I	40
#define EE_PID_PID7_D	41
#define EE_PID_PID8_P	42
#define EE_PID_PID8_I	43
#define EE_PID_PID8_D	44
#define EE_PID_PID9_P	45
#define EE_PID_PID9_I	46
#define EE_PID_PID9_D	47
#define EE_PID_PID10_P	48
#define EE_PID_PID10_I	49
#define EE_PID_PID10_D	50
#define EE_PID_PID11_P	51
#define EE_PID_PID11_I	52
#define EE_PID_PID11_D	53
#define EE_PID_PID12_P	54
#define EE_PID_PID12_I	55
#define EE_PID_PID12_D	56
//ErrorStatus  HSEStartUpStatus;
//FLASH_Status FlashStatus;
uint16_t VirtAddVarTab[NumbOfVar] = {0xAA00, 0xAA01, 0xAA02, 0xAA03, 0xAA04, 0xAA05, 0xAA06, 0xAA07, 0xAA08, 0xAA09, 0xAA0A, 0xAA0B, 0xAA0C, 0xAA0D, 0xAA0E, 0xAA0F,
																		 0xAA10, 0xAA11, 0xAA12, 0xAA13, 0xAA14, 0xAA15, 0xAA16, 0xAA17, 0xAA18, 0xAA19, 0xAA1A, 0xAA1B, 0xAA1C, 0xAA1D, 0xAA1E, 0xAA1F,
																		 0xAA20, 0xAA21, 0xAA22, 0xAA23, 0xAA24, 0xAA25, 0xAA26, 0xAA27, 0xAA28, 0xAA29, 0xAA2A, 0xAA2B, 0xAA2C, 0xAA2D, 0xAA2E, 0xAA2F,
																		 0xAA30, 0xAA31, 0xAA32, 0xAA33, 0xAA34, 0xAA35, 0xAA36, 0xAA37, 0xAA38,};
uint16_t temp;
void EE_INIT(void)
{
	EE_Init();
    EE_READ_PID();
    EE_READ_GYRO_OFFSET();
}
void EE_SAVE_ACC_OFFSET(void)
{
	EE_WriteVariable(VirtAddVarTab[EE_6050_ACC_X_OFFSET_ADDR], ACC_OFFSET.x);
	EE_WriteVariable(VirtAddVarTab[EE_6050_ACC_Y_OFFSET_ADDR], ACC_OFFSET.y);
	EE_WriteVariable(VirtAddVarTab[EE_6050_ACC_Z_OFFSET_ADDR], ACC_OFFSET.z);
}
void EE_READ_ACC_OFFSET(void)
{
	EE_ReadVariable(VirtAddVarTab[EE_6050_ACC_X_OFFSET_ADDR], &ACC_OFFSET.x);
	EE_ReadVariable(VirtAddVarTab[EE_6050_ACC_Y_OFFSET_ADDR], &ACC_OFFSET.y);
	EE_ReadVariable(VirtAddVarTab[EE_6050_ACC_Z_OFFSET_ADDR], &ACC_OFFSET.z);
}
void EE_SAVE_GYRO_OFFSET(void)
{
	EE_WriteVariable(VirtAddVarTab[EE_6050_GYRO_X_OFFSET_ADDR], GYRO_OFFSET.x);
	EE_WriteVariable(VirtAddVarTab[EE_6050_GYRO_Y_OFFSET_ADDR], GYRO_OFFSET.y);
	EE_WriteVariable(VirtAddVarTab[EE_6050_GYRO_Z_OFFSET_ADDR], GYRO_OFFSET.z);
}
void EE_READ_GYRO_OFFSET(void)
{
	EE_ReadVariable(VirtAddVarTab[EE_6050_GYRO_X_OFFSET_ADDR], &GYRO_OFFSET.x);
	EE_ReadVariable(VirtAddVarTab[EE_6050_GYRO_Y_OFFSET_ADDR], &GYRO_OFFSET.y);
	EE_ReadVariable(VirtAddVarTab[EE_6050_GYRO_Z_OFFSET_ADDR], &GYRO_OFFSET.z);
}
void EE_SAVE_PID(void)
{
    
	u16 _temp;
	_temp = PID_LDC1000.Kp * 1000;
	EE_WriteVariable(VirtAddVarTab[EE_PID_ROL_P],_temp);
	_temp = PID_LDC1000.Ti * 1000;
	EE_WriteVariable(VirtAddVarTab[EE_PID_ROL_I],_temp);
	_temp = PID_LDC1000.Td * 1000;
	EE_WriteVariable(VirtAddVarTab[EE_PID_ROL_D],_temp);

    _temp = PID_Gyro_Yaw_Struct.Kp * 1000;
	EE_WriteVariable(VirtAddVarTab[EE_PID_PIT_P],_temp);
	_temp = PID_Gyro_Yaw_Struct.Ti * 1000;
	EE_WriteVariable(VirtAddVarTab[EE_PID_PIT_I],_temp);
	_temp = PID_Gyro_Yaw_Struct.Td * 1000;
	EE_WriteVariable(VirtAddVarTab[EE_PID_PIT_D],_temp);

    _temp = PID_Position_Struct.Kp * 1000;
	EE_WriteVariable(VirtAddVarTab[EE_PID_YAW_P],_temp);
	_temp = PID_Position_Struct.Ti * 1000;
	EE_WriteVariable(VirtAddVarTab[EE_PID_YAW_I],_temp);
	_temp = PID_Position_Struct.Td * 1000;
	EE_WriteVariable(VirtAddVarTab[EE_PID_YAW_D],_temp);
    
	_temp= Set_Moto_Speed;
	EE_WriteVariable(VirtAddVarTab[EE_PID_PID12_P],_temp);
	_temp=Expect_ProximityData;
	EE_WriteVariable(VirtAddVarTab[EE_PID_PID12_I],_temp);
		_temp= Moto_Drive.moto_dir;
	EE_WriteVariable(VirtAddVarTab[EE_PID_PID12_D],_temp);
    
}
void EE_READ_PID(void)
{
	u16 _temp;
	EE_ReadVariable(VirtAddVarTab[EE_PID_ROL_P],&_temp);
	PID_LDC1000.Kp = (float)_temp / 1000;
	EE_ReadVariable(VirtAddVarTab[EE_PID_ROL_I],&_temp);
	PID_LDC1000.Ti = (float)_temp / 1000;
	EE_ReadVariable(VirtAddVarTab[EE_PID_ROL_D],&_temp);
	PID_LDC1000.Td = (float)_temp / 1000;
    
    EE_ReadVariable(VirtAddVarTab[EE_PID_PIT_P],&_temp);
	PID_Gyro_Yaw_Struct.Kp = (float)_temp / 1000;
	EE_ReadVariable(VirtAddVarTab[EE_PID_PIT_I],&_temp);
	PID_Gyro_Yaw_Struct.Ti = (float)_temp / 1000;
	EE_ReadVariable(VirtAddVarTab[EE_PID_PIT_D],&_temp);
	PID_Gyro_Yaw_Struct.Td = (float)_temp / 1000;
    
    EE_ReadVariable(VirtAddVarTab[EE_PID_YAW_P],&_temp);
	PID_Position_Struct.Kp = (float)_temp / 1000;
	EE_ReadVariable(VirtAddVarTab[EE_PID_YAW_I],&_temp);
	PID_Position_Struct.Ti = (float)_temp / 1000;
	EE_ReadVariable(VirtAddVarTab[EE_PID_YAW_D],&_temp);
	PID_Position_Struct.Td = (float)_temp / 1000;    
	
	EE_ReadVariable(VirtAddVarTab[EE_PID_PID12_P],&_temp);
	Set_Moto_Speed=_temp;
	EE_ReadVariable(VirtAddVarTab[EE_PID_PID12_I],&_temp);
	Expect_ProximityData=_temp;
	EE_ReadVariable(VirtAddVarTab[EE_PID_PID12_D],&_temp);
	Moto_Drive.moto_dir=_temp;

}
