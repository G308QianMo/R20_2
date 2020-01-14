#include "include.h"

/**
*@file    sensor.c
*@author	HaiXiaoNing
*@version 1.2
*@date    2020.1.14
*@brief   陀螺仪初始化、设置角度等函数
**/

static void GYRO_Reset_Module(void);
/**
*@function GYRO_Init
*@param    void
*@brief    陀螺仪进行多次进行初始化，初始化坐标值与角度值为零
*@retval   void
**/
void GYRO_Init(void)
{

    GYRO_Reset_Module(); //复位
	
    GYRO_Set_Angle(0);   //写入初值 
    GYRO_Set_Position(0, 0);
	
    delay_ms(20);        //延时
	
    GYRO_Set_Angle(0);   //再次写入初值  
    GYRO_Set_Position(0, 0);
	
    delay_ms(20);        //延时
	
    GYRO_Set_Angle(0);   //再次写入初值  
    GYRO_Set_Position(0, 0);
}

/**
*@function GYRO_Set_Angle
*@param    angle：设定角度值
*@brief    设置陀螺仪角度值，初始化时角度归零使用
*@retval   void
**/
void GYRO_Set_Angle(float angle)
{
    uDataConvert32TypeDef temp;
    CanTxMsg TxMessage;
    temp.float_form = angle;
    TxMessage.StdId = POS_CID;	  //
    TxMessage.DLC = 4;						//
    TxMessage.IDE = CAN_Id_Standard;
    TxMessage.RTR = CAN_RTR_Data;
    TxMessage.Data[0] = temp.u8_form[0];
    TxMessage.Data[1] = temp.u8_form[1];
    TxMessage.Data[2] = temp.u8_form[2];
    TxMessage.Data[3] = temp.u8_form[3];
    CAN_Transmit(CAN_BUS, &TxMessage);
}

/**
*@function GYRO_Set_Position
*@param    pos_x：设定x坐标值；pos_y：设定y坐标值
*@brief    设置陀螺仪坐标值，初始化坐标值归零时使用
*@retval   void
**/
void GYRO_Set_Position(int32_t pos_x, int32_t pos_y)
{
    CanTxMsg TxMessage;
    int32_t xx = 0, yy = 0;
    xx = -(int32_t)(pos_x* 12.8318);
    yy = -(int32_t)(pos_y* 12.5318);	
    TxMessage.StdId = POS_CID;				//??? ID
    TxMessage.DLC = 8;//??? 8
    TxMessage.IDE = CAN_Id_Standard;
    TxMessage.RTR = CAN_RTR_Data;
    TxMessage.Data[0] = (u8)(xx >> 0);
    TxMessage.Data[1] = (u8)(xx >> 8);
    TxMessage.Data[2] = (u8)(xx >> 16);
    TxMessage.Data[3] = (u8)(xx >> 24);
    TxMessage.Data[4] = (u8)(yy >> 0);
    TxMessage.Data[5] = (u8)(yy >> 8);
    TxMessage.Data[6] = (u8)(yy >> 16);
    TxMessage.Data[7] = (u8)(yy >> 24);
    CAN_Transmit(CAN_BUS, &TxMessage);
}

/**
*@function GYRO_Reset_Module
*@param    void
*@brief    陀螺仪软件复位
*@retval   void
**/
static void GYRO_Reset_Module(void)
{
    CanTxMsg TxMessage;
    TxMessage.StdId = POS_CID;		
    TxMessage.DLC = 2;					
    TxMessage.IDE = CAN_Id_Standard;
    TxMessage.RTR = CAN_RTR_Data;
    TxMessage.Data[0] = 0x55;
    TxMessage.Data[1] = 0xff;
    CAN_Transmit(CAN_BUS, &TxMessage);
}

