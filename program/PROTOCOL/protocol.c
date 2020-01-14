#include "include.h"

/**
*@file    protocol.c
*@author  蒋林杰
*@version 1.0
*@date    2019.11.23
*@brief   用于不同芯片之间的数据交流
**/

//数据拆分宏定义，在发送大于1字节的数据类型时，比如int16、float等，需要把数据拆分成单独字节进行发送

#define BYTE_0(dwTemp)       ( *( (char *)(&dwTemp)    ) )
#define BYTE_1(dwTemp)       ( *( (char *)(&dwTemp) + 1) )
#define BYTE_2(dwTemp)       ( *( (char *)(&dwTemp) + 2) )
#define BYTE_3(dwTemp)       ( *( (char *)(&dwTemp) + 3) )

uint8_t u8_data_to_send[100];					//数据发送缓存
static uint8_t static_u8_RxBuffer[100];			//接收数据缓存
static int16_t static_int_Processed_Data[50];	//处理后数据缓存
//该协议的各帧的定义
/*主机到从机发送帧
0     0x ee //帧头
1     0x ef //帧头
2     0x xx //功能字：1：自检
					  2：速度环
					  3：位置环
3     0x xx //帧长度
4~n-3 0x xx //数据帧
n-2   0x xx //sum校验和
n-1   0x fe //帧尾
n     0x ff //帧尾
*/


/**
 *@function Send_Data
 *@param    uint8_t *data 需传送至外部的数据
			uint8_t u8_length 需传送至外部的数据的个数
 *@brief    用于将数据传输出去API，可更改内部的数据传出方式
 *@retval   无
**/
void Send_Data(uint8_t *data, uint8_t u8_length)
{
	for (uint8_t i = 0; i < u8_length; i++)
	{
		USART_SendData(USART1, data[i]);
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
	}
}

/**
 *@function Data_Splitting_And_Sending
 *@param    int32_t *data 需拆分的数据
			uint8_t u8_length 需拆分数据的个数
			uint8_t function_num 确定当前拆分数据组的功能字
 *@brief    将需传输的数据拆分成定义的格式，并添加入相关信息
 *@retval   无
**/
void Data_Splitting_And_Sending(int16_t * data, uint8_t u8_length, uint8_t function_num)
{
	uint8_t u8_cnt = 0;									//计数
	uint8_t u8_sum = 0;									//校验和
	u8_data_to_send[u8_cnt++] = 0xEE;					//帧头
	u8_data_to_send[u8_cnt++] = 0xEF;					//帧头

	u8_data_to_send[u8_cnt++] = function_num;			//功能字
	u8_data_to_send[u8_cnt++] = u8_length * 2 + 4;		//数据长度

	for (int32_t i = 0; i < u8_length; i++)				//数据拆分
	{
		u8_data_to_send[u8_cnt++] = BYTE_1(data[i]);
		u8_data_to_send[u8_cnt++] = BYTE_0(data[i]);
	}
	for (uint8_t i = 0; i < u8_cnt; i++)
		u8_sum += u8_data_to_send[i];					//校验和
	u8_data_to_send[u8_cnt++] = u8_sum & 0xFF;			//得到校验和  
	u8_data_to_send[u8_cnt++] = 0xFE;					//帧尾
	u8_data_to_send[u8_cnt++] = 0xFF;					//帧尾
	Send_Data(u8_data_to_send, u8_cnt);					//数据发送
}



/**
 *@function Data_Check
 *@param    uint8_t *data 接收到的数据
			uint8_t num 接收到的数据的个数
 *@brief    将由Data_Recieving函数接收到的数据进行相关信息检查，若无误则进行下一步数据处理，调用 Data_Processing函数
			若有误，根据错误类型反馈调用 Data_Rerequest给数据发送端进行数据的再次请求
 *@retval   无
**/
void Data_Check(uint8_t *data, uint8_t num)
{
	uint8_t Check_flag = 0;
	uint8_t u8_sum = 0;

	if (data[0] != 0xEE || data[1] != 0xEF)
		Check_flag = 1;										//帧头错误
	if (data[num - 2] != 0xFE || data[num - 1] != 0xFF)
		Check_flag = 2;										//帧尾错误
	for (uint8_t i = 0; i < data[3]; i++)
		u8_sum += data[i];									//获得接收数据的校验和
	if ((u8_sum & 0xFF) != data[num - 3])
		Check_flag = 3;										//校验和错误														
	if (Check_flag == 0)									//若接收不出现错误，则进行下一步数据处理
		Data_Processing(static_u8_RxBuffer, num);			//调用数据处理函数
	else													//数据接收出现错误,根据不同的返回值确定错误类型
		Data_Rerequest(Check_flag);							//若出现数据错误，调用数据再请求函数
}


/**
 *@function Data_Recieving
 *@param    uint8_t data 来自于接收中断的数据
 *@brief    按步骤接收数据，接收完成后调用Data_Check函数进行数据检查，检查无误后进行数据处理
 *@retval   无
**/
static uint8_t static_u8_data_len = 0, static_u8_data_cnt = 0;
static uint8_t state = 0;

void Data_Recieving(uint8_t data)//数据接收，无需做任何改动
{
	if (state == 0 && data == 0xEE)					//帧头1
	{
		state = 1;
		static_u8_RxBuffer[0] = data;
	}
	else if (state == 1 && data == 0xEF)			//帧头2
	{
		state = 2;
		static_u8_RxBuffer[1] = data;
	}
	else if (state == 2 && data < 0xFF)				//功能帧
	{
		state = 3;
		static_u8_RxBuffer[2] = data;
	}
	else if (state == 3 && data < 100)				//数据长度
	{
		state = 4;
		static_u8_RxBuffer[3] = data;
		static_u8_data_len = data - 4;
		static_u8_data_cnt = 0;
	}
	else if (state == 4 && static_u8_data_len > 0)	//接收数据
	{
		static_u8_data_len--;
		static_u8_RxBuffer[4 + static_u8_data_cnt++] = data;
		if (static_u8_data_len == 0)
			state = 5;
	}
	else if (state == 5)							//接收校验和
	{
		state = 6;
		static_u8_RxBuffer[4 + static_u8_data_cnt++] = data;
	}
	else if (state == 6 && data == 0xFE)			//接收帧尾1
	{
		state = 7;
		static_u8_RxBuffer[4 + static_u8_data_cnt++] = data;
	}
	else if (state == 7 && data == 0xFF)			//接收帧尾2
	{
		state = 0;
		static_u8_RxBuffer[4 + static_u8_data_cnt++] = data;
		Data_Check(static_u8_RxBuffer, 4 + static_u8_data_cnt);//数据处理
	}
	else	state = 0;
}




/**
 *@function Data_Processing
 *@param    uint8_t *data 来自于接收中断的数据
			uint8_t length 接收到的数据的长度
 *@brief    将接收无误后的数据，根据功能字进行相应的数据处理
 *@retval   无
**/
void Data_Processing(uint8_t *data, uint8_t length)
{
	switch (data[2])			//data[2]为该帧数据的功能字帧
	{
	case 0xA1:
		for (uint8_t i = 0; i < (data[3] - 4) / 2; i++)
			static_int_Processed_Data[i] = (int32_t)(((uint16_t)data[4 + i * 2] << 8) + data[4 + i * 2 + 1]);
		break;
	}
}

/**
 *@function Data_Rerequest
 *@param    uint8_t erro_type 定义好的错误类型
 *@brief    若接收的数据有误，则发送数据再次请求
 *@retval   无
**/
void Data_Rerequest(uint8_t erro_type)
{
	uint8_t u8_sum = 0;

	u8_data_to_send[0] = 0xEE;
	u8_data_to_send[1] = 0xEF;//帧头

	u8_data_to_send[2] = 0xE0 | erro_type;//功能字
	u8_data_to_send[3] = 0;//数据长度

	for (uint8_t i = 0; i < 3; i++)
		u8_sum += u8_data_to_send[i];

	u8_data_to_send[3] = u8_sum;//得到校验和
	u8_data_to_send[4] = 0xFE;//帧尾
	u8_data_to_send[5] = 0xFF;//帧尾

	Send_Data(u8_data_to_send, 6);//发送数据请求
}
