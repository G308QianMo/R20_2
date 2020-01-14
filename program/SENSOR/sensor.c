#include "include.h"

/**
*@file    sensor.c
*@author	HaiXiaoNing
*@version 1.2
*@date    2020.1.14
*@brief   采集底盘坐标值与角度值，并进行坐标解算，其中角度值有IMU和陀螺仪采集两种，坐标采集使用编码器
**/


//注意此处的数据格式不能乱改
__IO float global_gyro_angle_last = 0;
__IO float global_gyro_angle_now = 0;
__IO float global_gyro_angle = 0;
__IO int32_t global_gyro_pulse_x = 0;
__IO int32_t global_gyro_pulse_y = 0;
__IO float global_enc_x = 132 / 0.0392699081698724, global_enc_y = -13 / 0.0392699081698724;
__IO int32_t global_enc_dx = 0, global_enc_dy = 0;
__IO EncodePointTypeDef global_gyro_location;
int global_DT35_dataA, global_DT35_dataB, global_DT35_dataC;
float e[3];//IMU读回角度值，放在了e[2]中
int get_initial_value=1;//IMU初始值标志位
float global_imu_angle_initial_value=0;



/**
*@function GYRO_Get_Location_Param
*@param    void
*@brief    得到传感器的值（陀螺仪、码盘、激光）
*@retval   void
**/
void GYRO_Get_Location_Param(void)
{
	uDataConvert32TypeDef temp;
	CanRxMsg RxMessage;
	if (CAN_GetITStatus(CAN2, CAN_IT_FMP1) != RESET)
	{
		CAN_Receive(CAN2, CAN_FIFO1, &RxMessage);
		switch (RxMessage.StdId)
		{
			//得到陀螺仪传回的角度值
		case GYRO_ID:						      //GYRO_ID==0x11			
			if (RxMessage.DLC == 4)
			{
				temp.u8_form[0] = RxMessage.Data[0];
				temp.u8_form[1] = RxMessage.Data[1];
				temp.u8_form[2] = RxMessage.Data[2];
				temp.u8_form[3] = RxMessage.Data[3];
				memcpy((void *)&global_gyro_angle_now, &temp.float_form, 4);
			}
			else if (RxMessage.DLC == 8)	//
			{
				//										temp.u8_form[0] = RxMessage.Data[0];
				//										temp.u8_form[1] = RxMessage.Data[1];
				//										temp.u8_form[2] = RxMessage.Data[2];
				//										temp.u8_form[3] = RxMessage.Data[3];
				//										memcpy((void *)&GYRO_pulse_x, &temp.s32_form, 4);

				//										temp.u8_form[0] = RxMessage.Data[4];
				//										temp.u8_form[1] = RxMessage.Data[5];
				//										temp.u8_form[2] = RxMessage.Data[6];
				//										temp.u8_form[3] = RxMessage.Data[7];
				//										memcpy((void *)&GYRO_pulse_y, &temp.s32_form, 4);




				global_gyro_pulse_y = TIM1->CNT;
				global_enc_dy = global_gyro_pulse_y - 30000;
				global_gyro_pulse_x = TIM2->CNT;
				global_enc_dx = -(global_gyro_pulse_x - 30000);


				TIM2->CNT = 30000;
				TIM1->CNT = 30000;

				global_gyro_angle = global_gyro_angle_now / 2 + global_gyro_angle_last / 2;
				global_enc_x = global_enc_x + (float)global_enc_dx * my_cos(global_gyro_angle) - (float)global_enc_dy * my_sin(global_gyro_angle);//- 94.46;
				global_enc_y = global_enc_y + (float)global_enc_dx * my_sin(global_gyro_angle) + (float)global_enc_dy * my_cos(global_gyro_angle);//- 224.62;

				global_gyro_location.x = (float)global_enc_x*CODER_PARAM - (121.75f* my_sin(global_gyro_angle - 73.5227f) + 245.75f);
				global_gyro_location.y = (float)global_enc_y*CODER_PARAM - (119.43*my_sin(global_gyro_angle - 157.4137f) + 33.87f);
				global_gyro_location.angle = global_gyro_angle_now;
				global_gyro_angle_last = global_gyro_angle_now;

			}
			break;
		case 0x13:		                 //global_DT35_dataA_ID
			temp.u8_form[0] = RxMessage.Data[0];
			temp.u8_form[1] = RxMessage.Data[1];
			temp.u8_form[2] = RxMessage.Data[2];
			temp.u8_form[3] = RxMessage.Data[3];
			memcpy((void *)&global_DT35_dataA, &temp.s32_form, 4);
			break;
		case 0x15:		                //global_DT35_dataB_ID
			temp.u8_form[0] = RxMessage.Data[0];
			temp.u8_form[1] = RxMessage.Data[1];
			temp.u8_form[2] = RxMessage.Data[2];
			temp.u8_form[3] = RxMessage.Data[3];
			memcpy((void *)&global_DT35_dataB, &temp.s32_form, 4);
			break;
		case 0x16:		                //global_DT35_dataC_ID
			temp.u8_form[0] = RxMessage.Data[0];
			temp.u8_form[1] = RxMessage.Data[1];
			temp.u8_form[2] = RxMessage.Data[2];
			temp.u8_form[3] = RxMessage.Data[3];
			memcpy((void *)&global_DT35_dataC, &temp.s32_form, 4);
			break;
		default:
			break;
		}

		CAN_ClearITPendingBit(CAN2, CAN_IT_FMP1);
	}
}

/**
*@function IMU_Get_Location_Param
*@param    void
*@brief    通过IMU采集角度解算坐标,在串口3的中断中使用
*@retval   void
**/
void IMU_Get_Location_Param(void)
{
	      get_eular(e);
	      global_gyro_angle_now = e[2] - global_imu_angle_initial_value; //IMU采集到的角度值
	      global_gyro_angle_now = -global_gyro_angle_now;
				global_gyro_pulse_y = TIM1->CNT;
				global_enc_dy = global_gyro_pulse_y - 30000;
				global_gyro_pulse_x = TIM2->CNT;
				global_enc_dx = -(global_gyro_pulse_x - 30000);


				TIM2->CNT = 30000;
				TIM1->CNT = 30000;

				global_gyro_angle = global_gyro_angle_now / 2 + global_gyro_angle_last / 2;
				global_enc_x = global_enc_x + (float)global_enc_dx * my_cos(global_gyro_angle) - (float)global_enc_dy * my_sin(global_gyro_angle);//- 94.46;
				global_enc_y = global_enc_y + (float)global_enc_dx * my_sin(global_gyro_angle) + (float)global_enc_dy * my_cos(global_gyro_angle);//- 224.62;

				global_gyro_location.x = (float)global_enc_x*CODER_PARAM - (121.75f* my_sin(global_gyro_angle - 73.5227f) + 245.75f);
				global_gyro_location.y = (float)global_enc_y*CODER_PARAM - (119.43*my_sin(global_gyro_angle - 157.4137f) + 33.87f);
				global_gyro_location.angle = global_gyro_angle_now;
				global_gyro_angle_last = global_gyro_angle_now;

}
