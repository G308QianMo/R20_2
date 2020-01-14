  /*
 * ANO_DT.h
 *
 *  Created on: 2019.10.04
 *      Author: LiuCX
 */

#ifndef __ANO_DT_H
#define __ANO_DT_H

#include "stm32f4xx.h"
#include <stdint.h>

#define BYTE0(dwTemp)       ( *( (char *)(&dwTemp) + 0) )
#define BYTE1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )



/**
  * @name   Argument_Load
  * @param  None
  * @retval None
  * @brief  从FLASH里读取PID数据（不用）
  */
void Argument_Load(void);

void Argument_Init(void);

// Uart 命令行保存PID参数
void Uart_Commond_Save(void);
/**
  * @name   ANO_DT_Send_Status
  * @param  尚未看懂
  * @retval None
  * @brief  发送 姿态
  */
void ANO_DT_SendStatus(float angle_rol, float angle_pit, float angle_yaw,
                        s32 alt, uint8_t fly_model, uint8_t armed);
void ANO_DT_Send_RCData(u16 thr,u16 yaw,u16 rol,u16 pit,u16 aux1,u16 aux2,u16 aux3,u16 aux4,u16 aux5,u16 aux6);
/**
  * @name   ANO_DT_Send_Senser
  * @param  各传感器的数据
  * @retval None
  * @brief  传感器数据---此处用于给示波器发送数据
  */
void ANO_DT_Send_Senser(s16 a_x,s16 a_y,s16 a_z,
                        s16 g_x,s16 g_y,s16 g_z,
                        s16 m_x,s16 m_y,s16 m_z
                        ,s32 bar);

 uint8_t ANO_DT_Data_Receive_Prepare(uint8_t data);

#endif /* __ANO_DT_H */
