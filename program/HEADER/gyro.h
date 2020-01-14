#ifndef _GYRO_H_
#define _GYRO_H_
#include "stm32f4xx.h"
#include <stdint.h>

typedef union
{
    uint8_t  u8_form[4];
    uint32_t u32_form;
    int32_t  s32_form;
    float    float_form;
}uDataConvert32TypeDef;


#define POS_CID 0x12		//控制器ID
#define GYRO_ID 0x11    //陀螺仪模块ID 
#define GYRO_USE_CAN2
#define CODER_PARAM (double)0.0392699081698724   //???????50.8 * p / ???2000

#if defined (GYRO_USE_CAN1)
#define CAN_BUS CAN1
#elif defined (GYRO_USE_CAN2)
#define CAN_BUS CAN2
#else
#error "Please choose GYRO_CAN_Bus!"
#endif

void GYRO_Set_Angle(float angle);
void GYRO_Set_Position(int32_t pos_x, int32_t pos_y);
void GYRO_Init(void);

#endif


