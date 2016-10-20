#include "bankCommon.h"

CONTROL_MODE Control_Mode = SLAM_MODE;
RobotEncoder	robot_encoder_data;

int heartbreak_cnt = 0;


/**
  * @brief  ????????????
  * @param  *buffer: ??? 
	* @param        c: ??
  * @retval ???????
  */
char findCharPosFromBegin(unsigned char * buffer,unsigned char c)
{
	 int i;
	 int len = strlen((const char *)buffer);
	 len = 22;
	 for(i = 0; i < len ;i++)
	 {
		  if(buffer[i] == c)
			{
				 return i;
			}
	 }
	 return -1;
}

/**
  * @brief  ????????????
  * @param  *buffer: ??? 
	* @param        c: ??
  * @retval ???????
  */
char findCharPosFromEnd(unsigned char * buffer,unsigned char c)
{
	int i,len;
	len = strlen((const char *)buffer);
	for(i = len -1;i >= 0;i--)
	{
		if(buffer[i] == c)
			return i;
	}
	return -1;
}

/**
  * @brief  ????
  * @param  *buffer: ??????? 
	* @param      len: ?????
  * @retval ??????
  */
unsigned char XorCode(unsigned char *buffer,int len)
{
	int i;
	unsigned char ans = buffer[0];
	for(i = 1; i < len;i++)
	{
		 ans ^= buffer[i];
	}
	return ans & 0x7f;
}




