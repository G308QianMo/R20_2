#ifndef __SENSOR_H
#define __SENSOR_H
/**
*@file    sensor.h
*@author	HaiXiaoNing
*@version 1.2
*@date    2020.1.13
*@brief   进行硬件初始化配置
**/

#define CODER_PARAM (double)0.0392699081698724  


void GYRO_Get_Location_Param(void);//利用陀螺仪采集角度，解算坐标
void IMU_Get_Location_Param(void);//利用IMU采集角度，解算坐标
#endif
