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
#include "interchip.h"
#include "ctrl_bus_cmd.h"
#include "virtual_motor.h"
#include "robotmodel.h"
#include "motorDriver.h"
#include "avoidobstacle.h"
#include <string.h>

#include "timer.h"
#include "bluetooth.h"
#include "dbus.h"


int dt = 0;
int templ = 0,tempr = 0;
static void onRequestSlamcoreCB(infra_channel_desc_t * channel)
{
    slamcore_cb_cmd_t * request = (slamcore_cb_cmd_t *)_request_data;
    switch (request->cmd)
    {
        case SLAMWARECORECB_CMD_CONNECT_BASE:   //请求连接
        {
            base_connect_request_t  *req_pkt = (base_connect_request_t *)request->payload;
            if (req_pkt->protocol_version != SLAMWARECORE_PROTOCOL_VERSION)
                goto on_error;

            base_connect_response_t ans_pkt;
            memcpy(&ans_pkt.model, "virt_base", 12);
            ans_pkt.firmware_version = 0x1;
            ans_pkt.hardware_version = 0x1;
            memcpy((char *)&ans_pkt.serial_number, "736c616d3031", 12);//slam01

            net_send_ans(channel, &ans_pkt, sizeof(base_connect_response_t));
        }
        break;
        case SLAMWARECORECB_CMD_GET_BASE_CONF: //请求外部参数设置
        {
            base_conf_response_t ans_pkt;
            ans_pkt.base_type = CIRCLE;																						//机器的外形为圆形
            ans_pkt.base_radius_q8 = (_u32)(1000 * 256.0);       									//机器的半径为400mm
            ans_pkt.base_motor_type = TWO_WHEEL;                									//两轮结构
            ans_pkt.base_sensor_num = 3;                        									//壁障传感器数量
            memset((char *)&ans_pkt.base_sensors, 0, sizeof(base_pos_t)*8);
            ans_pkt.base_bumper_num = 0;																					//碰撞传感器数量
            memset((char *)&ans_pkt.base_bumpers, 0, sizeof(base_pos_t)*8);
			
            // base_sensors  设置传感器的安装坐标和方位  正前方
            ans_pkt.base_sensors[0].x_to_center_mm_q8 = (_s32)(54 * 256.0);
            ans_pkt.base_sensors[0].y_to_center_mm_q8 = (_s32)(0 * 256.0);
            ans_pkt.base_sensors[0].z_to_center_mm_q8 = (_s32)(0.0 * 256.0);
            ans_pkt.base_sensors[0].clockwise_angle_to_center_degree_q8 = (_s32)(0 * 256.0);
          
					  //右前方
            ans_pkt.base_sensors[1].x_to_center_mm_q8 = (_s32)(54 * 256.0);
            ans_pkt.base_sensors[1].y_to_center_mm_q8 = (_s32)(-82 * 256.0);
            ans_pkt.base_sensors[1].z_to_center_mm_q8 = (_s32)(0.0 * 256.0);
            ans_pkt.base_sensors[1].clockwise_angle_to_center_degree_q8 = (_s32)(-28.7 * 256.0);
          
					  //左前方
            ans_pkt.base_sensors[2].x_to_center_mm_q8 = (_s32)(54 * 256.0);
            ans_pkt.base_sensors[2].y_to_center_mm_q8 = (_s32)(82.0 * 256.0);
            ans_pkt.base_sensors[2].z_to_center_mm_q8 = (_s32)(0.0 * 256.0);
            ans_pkt.base_sensors[2].clockwise_angle_to_center_degree_q8 = (_s32)(28.7 * 256.0);
						
            net_send_ans(channel, &ans_pkt, sizeof(base_conf_response_t));
        }
        break;
        case SLAMWARECORECB_CMD_GET_BASE_STATUS:      //返回电池电量和充电状态
        {
            base_status_response_t ans_pkt;

            ans_pkt.battery_is_charging = 0;
            ans_pkt.battery_percentage = 90;
            net_send_ans(channel, &ans_pkt, sizeof(base_status_response_t));
        }
        break;
        case SLAMWARECORECB_CMD_GET_BASE_MOTOR_DATA:  //返回电机编码器值
        {
					  
						base_motor_status_response_t ans_pkt;
						ans_pkt.base_dx_mm_q16 = (_s32)(robotPose.x * 65536.0);
						ans_pkt.base_dy_mm_q16 = (_s32)(robotPose.y * 65536.0);
						ans_pkt.base_dtheta_degree_q16 = (_s32)(robotPose.theta * 65536.0);
					
						robotPose.x = robotPose.y = robotPose.theta = 0;                              //数据清0
								
            ans_pkt.motor_cumulate_dist_mm_q16[0] = (int)(robotPose.ldistance );
						ans_pkt.motor_cumulate_dist_mm_q16[1] = (int)(robotPose.rdistance );
						ans_pkt.motor_cumulate_dist_mm_q16[2] = (_s32)0;
						ans_pkt.motor_cumulate_dist_mm_q16[3] = (_s32)0;
            net_send_ans(channel, &ans_pkt, sizeof(base_motor_status_response_t));
        }
        break;
        case SLAMWARECORECB_CMD_GET_BASE_SENSOR_DATA: //返回距离传感器数值 
        {
            base_sensor_data_response_t ans_pkt;
            
            ans_pkt.sensor_data_mm_q16[0] = (_u32)(dist_forward * 65536.0);
            ans_pkt.sensor_data_mm_q16[1] = (_u32)(dist_right * 65536.0);
            ans_pkt.sensor_data_mm_q16[2] = (_u32)(dist_left * 65536.0);
            ans_pkt.sensor_data_mm_q16[3] = (_u32)(6000.0 * 65536.0);
            ans_pkt.sensor_data_mm_q16[4] = (_u32)(6000.0 * 65536.0);
            ans_pkt.sensor_data_mm_q16[5] = (_u32)(6000.0 * 65536.0);
            ans_pkt.sensor_data_mm_q16[6] = (_u32)(6000.0 * 65536.0);
            ans_pkt.sensor_data_mm_q16[7] = (_u32)(6000.0 * 65536.0);
            
					  dist_forward = 0;
					  dist_right = 0;
					  dist_left = 0;
					
					
            ans_pkt.sensor_data_mm_q16[8] = (_u32)(6000.0 * 65536.0);
            ans_pkt.sensor_data_mm_q16[9] = (_u32)(6000.0 * 65536.0);
            ans_pkt.sensor_data_mm_q16[10] = (_u32)(6000.0 * 65536.0);
            
            net_send_ans(channel, &ans_pkt, sizeof(base_sensor_data_response_t));
        }
        break;
        case SLAMWARECORECB_CMD_GET_BASE_BUMPER_DATA: //返回碰撞传感器状态
        {
            base_bumper_data_response_t ans_pkt;
            ans_pkt.bumper_data = 0xff;
            net_send_ans(channel, &ans_pkt, sizeof(base_bumper_data_response_t));
        }
        break;
        case SLAMWARECORECB_CMD_SET_BASE_MOTOR: //设置小车运动参数
        {
            base_set_motor_request_t *ans_pkt = (base_set_motor_request_t *)request->payload;
					  robotSpd.target.lspd =  ans_pkt->motor_speed_mm[0];
					  robotSpd.target.rspd = ans_pkt->motor_speed_mm[1];
				
						//位于SLAM模式才需要响应上面的速度指令
						if(Control_Mode == SLAM_MODE)
					  {
							double spd[3];
							Robot_Move(ans_pkt->motor_speed_mm[0],ans_pkt->motor_speed_mm[1],spd);
							Bluetooth_Printf(USART3,"spd before: %.1f %.1f %.1f\r\n",spd[0],spd[1],spd[2]);
							if(ans_pkt->motor_speed_mm[0] != 0 || ans_pkt->motor_speed_mm[1] != 0)
							{
								SonarAvoidObstacle(sonar,spd);
//								if(sonar[0] < 150 || sonar[1] < 150 || sonar[2] < 150)
//								{
//									spd[0] = spd[1] = spd[2] = 0;
//								}
							}
							Bluetooth_Printf(USART3,"spd after: %.1f %.1f %.1f\r\n",spd[0],spd[1],spd[2]);
							Motor_setRun(spd[0],spd[1],spd[2]);
            }
						net_send_ans(channel, NULL, 0);
        }
        break;
        
        
        case SLAMWARECORECB_CMD_POLL_BASE_ANS_CMD: //外部系统命令查询请求
        {
            net_send_ans(channel, NULL, 0);
        }
        break;  
      
        case SLAMWARECORECB_CMD_SEND_EVENT:
        {
         //    slamcore_event_t *ans_pkt = (slamcore_event_t *)request->payload;
             net_send_ans(channel, NULL, 0);
        }
        break;
        
        
       default:
           goto on_error;
    }
    return;
on_error:
    net_send_errorcode(channel,PKT_ERRORCODE_NOT_SUPPORT);
}


void onHostRequest(infra_channel_desc_t* channel)
{
    switch (net_get_request_cmd(channel))
    {
        case CMD_CODE_SLAMWARECORE_CTRL_BUS:
            onRequestSlamcoreCB(channel);
        break;

        default:
            net_send_errorcode(channel, PKT_ERRORCODE_NOT_SUPPORT);
    }
}
