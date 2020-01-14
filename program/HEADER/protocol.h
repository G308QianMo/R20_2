#ifndef __PROTOCOL_H
#define __PROTOCOL_H
#include "include.h"




void Send_Data(uint8_t *dataToSend, uint8_t length);//数据发送
void Data_Splitting_And_Sending(int16_t * data, uint8_t length, uint8_t function_num);//数据拆分
void Data_Check(uint8_t *data, uint8_t num);//数据检查
void Data_Recieving(uint8_t data);//数据接收
void Data_Processing(uint8_t *data, uint8_t length);//数据处理
void Data_Rerequest(uint8_t erro_type);//数据重请求



#endif
