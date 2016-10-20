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

#ifndef CTRL_BUS_CMD_H
#define CTRL_BUS_CMD_H

// Protocol Packages
#if defined(_WIN32) || defined(__ICCARM__)
#pragma pack(1)
#endif

#include "types.h"

#define SLAMWARECORE_PROTOCOL_VERSION              (0x1)

// EXTEND MODE Command for slamware core ctrl bus
#define CMD_CODE_SLAMWARECORE_CTRL_BUS             (0xF8)

#define SLAMWARECORECB_CMD_CONNECT_BASE            (0x10)
#define SLAMWARECORECB_CMD_GET_BASE_CONF           (0x20)
#define SLAMWARECORECB_CMD_GET_BASE_STATUS         (0x30)
#define SLAMWARECORECB_CMD_GET_BASE_MOTOR_DATA     (0x31)
#define SLAMWARECORECB_CMD_GET_BASE_SENSOR_DATA    (0x32)
#define SLAMWARECORECB_CMD_GET_BASE_BUMPER_DATA    (0x33)
#define SLAMWARECORECB_CMD_SET_BASE_MOTOR          (0x40)
#define SLAMWARECORECB_CMD_POLL_BASE_CMD           (0x50)
#define SLAMWARECORECB_CMD_POLL_BASE_ANS_CMD       (0x5f)
#define SLAMWARECORECB_CMD_SEND_EVENT              (0x60)


// Protocol Packages
typedef struct _slamcore_cb_cmd_t
{
    _u8 cmd;
    _u8 payload[1];
} __attribute__((packed)) slamcore_cb_cmd_t;

typedef struct _base_connect_request
{
    _u8  protocol_version;
} __attribute__((packed)) base_connect_request_t;

typedef struct _base_connect_response
{
    _u8  model[12];
    _u16 firmware_version;
    _u16 hardware_version;
    _u32 serial_number[3];
} __attribute__((packed)) base_connect_response_t;

enum _base_type {
    CIRCLE = 0,
    SQUARE = 1,
};

enum _base_motor_type {
    TWO_WHEEL = 0,
	THREE_WHEEL=1,
};

typedef struct _base_sensor_pos
{
    _s32  x_to_center_mm_q8;
    _s32  y_to_center_mm_q8;
    _s32  z_to_center_mm_q8;
    _u32  clockwise_angle_to_center_degree_q8;
} __attribute__((packed)) base_pos_t;

typedef struct _base_conf_response
{
    _u8        base_type;
    _u32       base_radius_q8;
    _u8        base_motor_type;
    _u8        base_sensor_num;
    base_pos_t base_sensors[8];
    _u8        base_bumper_num;
    base_pos_t base_bumpers[8];
} __attribute__((packed)) base_conf_response_t;

typedef struct _base_status_response
{
    _u8 battery_percentage;
    _u8 battery_is_charging;
} __attribute__((packed)) base_status_response_t;

typedef struct _base_motor_status_response
{
    _s32 motor_cumulate_dist_mm_q16[4];
    _s32 base_dx_mm_q16;
    _s32 base_dy_mm_q16;
    _s32 base_dtheta_degree_q16;
} __attribute__((packed)) base_motor_status_response_t;

typedef struct _base_sensor_data_response
{
    _u32  sensor_data_mm_q16[16];
} __attribute__((packed)) base_sensor_data_response_t;

typedef struct _base_bumper_data_response
{
    _u8  bumper_data; // one bit for one bumper sensor, at most 8
} __attribute__((packed)) base_bumper_data_response_t;

typedef struct _base_set_motor_request
{
    _s32 motor_speed_mm[4];
} __attribute__((packed)) base_set_motor_request_t;


typedef struct _slamcore_event_t
{
    _u8 slamcore_event;
} __attribute__((packed)) slamcore_event_t;


#if defined(_WIN32) || defined(__ICCARM__)
#pragma pack()
#endif

#endif

