/*
 * SlamTec Ctrl Bus Protocol
 * Copyright 2009 - 2015 RoboPeak
 * Copyright 2013 - 2015 Shanghai SlamTec Co., Ltd.
 * http://www.slamtec.com
 * All rights reserved.
 */
/*
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, 
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice, 
 *    this list of conditions and the following disclaimer in the documentation 
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY 
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES 
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT 
 * SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT 
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR 
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "virtual_motor.h"

#include "timer.h"

int32_t virtual_L_wheel_speed;
int32_t virtual_R_wheel_speed;
float virtual_L_wheel_distance;
float virtual_R_wheel_distance;

/*
*    virtual motor wheels speed and distance variables
*/
void virtualmotor_init(void)
{
    virtual_L_wheel_speed = 0;
    virtual_R_wheel_speed = 0;
    virtual_L_wheel_distance = 0;
    virtual_R_wheel_distance = 0;
}


static uint32_t virtualspeed_frequency = 0;
/*
*    get virtual motor left and right speed in 100ms, then Calculation Each  total distance
*/
void virtualspeed_heartbeat(void)
{
    if((millis() - virtualspeed_frequency) >= 100)
    {
       virtualspeed_frequency = millis();
       if(virtual_L_wheel_speed >= 0)
           virtual_L_wheel_distance += ((float)virtual_L_wheel_speed * 0.1);
       else
          virtual_L_wheel_distance -= ((float)virtual_L_wheel_speed * 0.1);
    
       if(virtual_R_wheel_speed >= 0)
          virtual_R_wheel_distance += ((float)virtual_R_wheel_speed * 0.1);
       else
          virtual_R_wheel_distance -= ((float)virtual_R_wheel_speed * 0.1);
   }
}

/*
*    set virtual motor left and right speed ,unit : mm/s
*/
void virtualmotor_set_speed(int32_t lspeed,int32_t rspeed)
{
    virtual_L_wheel_speed = lspeed;
    virtual_R_wheel_speed = rspeed;
}

/*
*    get virtual motor left and right wheel total distance,unit : mm
*/
float virtualmotor_get_distance(_u8 which)
{
    switch(which)
    {
        case 0:
            return virtual_L_wheel_distance;
            break;
        case 1:
            return virtual_R_wheel_distance;
            break;
        default:
            break; 
    }
    return 0.0;
}
