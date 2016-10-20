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

#ifndef NET_COMMON_H
#define NET_COMMON_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "types.h"

#if defined(_WIN32) || defined(__ICCARM__)
#pragma pack(1)
#endif

#define BIT_FLAG_CONTD     0
#define BIT_FLAG_ERRORBIT  1
#define BIT_FLAG_CUSTOMBIT 2
#define BIT_FLAG_TESTBIT   3
#define BIT_FLAG_CHECKSUM  4
#define BIT_FLAG_ADDR_EN   5
#define BIT_FLAG_LONGFRAME 6
#define BIT_FLAG_EXTFLAG   7

#define MSK_FLAG_CONTD     (0x1<<BIT_FLAG_CONTD)
#define MSK_FLAG_ERRORBIT  (0x1<<BIT_FLAG_ERRORBIT)
#define MSK_FLAG_CUSTOMBIT (0x1<<BIT_FLAG_CUSTOMBIT)
#define MSK_FLAG_TESTBIT   (0x1<<BIT_FLAG_TESTBIT)
#define MSK_FLAG_CHECKSUM  (0x1<<BIT_FLAG_CHECKSUM)
#define MSK_FLAG_ADDR_EN   (0x1<<BIT_FLAG_ADDR_EN)
#define MSK_FLAG_LONGFRAME (0x1<<BIT_FLAG_LONGFRAME)
#define MSK_FLAG_EXTFLAG   (0x1<<BIT_FLAG_EXTFLAG)

typedef union frame_flag
{
    _u8  flat;
    //frame_flag_bits_t bits;
} __attribute__((packed)) frame_flag_t;

//--------------------------------------------

typedef union frame_extflag
{
    _u8  flat;
  //frame_extflag_bits_t bits;
} __attribute__((packed)) frame_extflag_t;

//--------------------------------------------

#define BIT_ADDR_EXTBIT         7
#define MSK_ADDR_EXTBIT         (0x1<<BIT_ADDR_EXTBIT)
#define MSK_ADDR_ID             0x7

typedef union frame_addr
{
    _u8  flat;
    //frame_address_bits_t bits;
} __attribute__((packed)) frame_addr_t;

//-----------------------------------------------------------------

#if defined(_WIN32) || defined(__ICCARM__)
#pragma pack()
#endif

// get the device descriptor from the device
#define CMD_CTRL_ENUM	 0x00

// reset the device
#define CMD_CTRL_RESET	 0x01

// make the device to specific sleep level
#define CMD_CTRL_SLEEP	 0x02

// get the health status of the device
#define CMD_CTRL_HEALTH  0x03

// get some attributes to get some detailed device specific attributes
#define CMD_CTRL_GETATTR 0x04

// do some io control operation
#define CMD_CTRL_IOCTL	 0x05

#define CMD_CODE_SYNC    0x00
#define CMD_CODE_ECHO    0x01

#define STATUS_CODE_SYNC 0x00
#define STATUS_CODE_ECHO 0x01
#define STATUS_CODE_ANS  0x02
#define STATUS_CODE_ANS_RXERR  0x03
//fake code
#define STATUS_CODE_BAD_PKT 0xFF    

#define PKT_ERRORCODE_NULL            0
#define PKT_ERRORCODE_CHECKSUM_FAIL 0x40
#define PKT_ERRORCODE_SIZE_OVERFLOW 0x20
#define PKT_ERRORCODE_NOT_SYNC_PKT  0x10
#define PKT_ERRORCODE_NOT_SUPPORT   0x8000
#define PKT_ERRORCODE_BAD_CMD              0x8001
#define PKT_ERRORCODE_OPERATION_FAIL       0x8002


#ifdef __cplusplus
}
#endif

#endif

