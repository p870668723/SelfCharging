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
#include "common.h"
#include "serial_channel.h"
#include "types.h"
#include "uart.h"
#include "string.h"
static infra_channel_desc_t _channel;
static interchip_rx_proc_t  _rx_proc;

static _u8 _serial_rx_buffer[CONFIG_DRV_SERIALCHANNEL_RXBUFFERSIZE];
static _u8 _serial_tx_buffer[CONFIG_DRV_SERIALCHANNEL_TXBUFFERSIZE];

static void _serial_on_rx_t(_u8 data)
{
    if (_rx_proc) 
    {
        _u8 data_u8 = data;
        _rx_proc(&_channel, &data_u8, 1);
    }
}

static void _register_rx_callback(infra_channel_desc_t * handle, interchip_rx_proc_t proc)
{
    _rx_proc = proc;
}

static int  _tx_buffer_set( infra_channel_desc_t * handle, const void * data, _u32 size, _u32  offset)
{
    memcpy(_serial_tx_buffer + offset, data, size);
    return 1;
}

static void _tx_flush(infra_channel_desc_t * handle, _u32 size)
{
    put_bytes(_serial_tx_buffer, size);
}

int drv_serialchannel_init(void)
{

     uart_init_115200();

    _rx_proc = NULL;
    _channel.rxbuffer = _serial_rx_buffer;
    _channel._medialayer_data = 0;
    _channel.register_rx_callback = &_register_rx_callback;
    _channel.tx_buffer_set = &_tx_buffer_set;
    _channel.tx_flush = &_tx_flush;
    
    _channel.rxbuffersize = sizeof(_serial_rx_buffer);
    _channel.txbuffersize = CONFIG_DRV_SERIALCHANNEL_TXBUFFERSIZE;
    
    serial_setrecv_backcall( _serial_on_rx_t);
    
    return 1;
}


void drv_serialchannel_shutdown(_u8 id)
{
   serial_setrecv_backcall( NULL);
}


infra_channel_desc_t * drv_serialchannel_getchannel()
{
    return &_channel;
}
