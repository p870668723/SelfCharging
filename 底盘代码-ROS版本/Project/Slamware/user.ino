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
/*
 * SlamTec Ctrl Bus Protocol Arduino Example
 * This example shows the easy and common way to Communication with SLAMCORE
 * 
 * You may freely add your application code based on this template
 *
 * USAGE:
 * ---------------------------------
 * 1. Download this sketch code to your Arduino board
 * 2. Connect the SLAMCORE's serial port (RX/TX/GND) to your Arduino board (Pin 0 and Pin1)
 */
 
#include "common.h"
#include "led.h"
#include "serial_channel.h"
#include "virtual_motor.h"
#include "uart.h"

extern void onHostRequest(infra_channel_desc_t* channel);

/*
*    heartbeat handle
*    get virtual motor left and right speed in 100ms, then Calculation Each  total distance
*    led heartbeat handle,just for monitor program running ,can be cut
*/
void dev_heartbeat(void)
{
    virtualspeed_heartbeat();
    led_blink();
}


void setup() {  
 
    drv_serialchannel_init();					//initialization for slamcore communication with serialchannel : uart1
    net_bind(drv_serialchannel_getchannel());			//serialchannel  bind  interchip protocol
    led_init();							//led initialization,can be cut
    virtualmotor_init();					//virtual motor run variables initialization
}

void loop() {
  
    if (net_poll_request(drv_serialchannel_getchannel())) 	//Query slamcore Request Frame
    {
        onHostRequest(drv_serialchannel_getchannel());		//Response slamcore Request Frame
    } 
    dev_heartbeat(); 						//heartbeat handle
}

