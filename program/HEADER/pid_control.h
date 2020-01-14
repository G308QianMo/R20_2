#ifndef __PID_CONTROL_H
#define __PID_CONTROL_H

#include "pid_parameter.h"

void PID_Set_Value_Angle(int32_t Angle);
void PID_Set_Value_X(int32_t X);
void PID_Set_Value_Y(int32_t Y);


void PID_Loop_X(void);  
void PID_Loop_Y(void);
void PID_Loop_angle(void);


int32_t PID_Cal(PID_Loop_Def * PID_Loop,int32_t Real_Value);
void PID_Setup(int32_t Angle_P,  int32_t Angle_D,  int32_t Angle_Max,  int32_t Angle_Min,
	           int32_t X_P,  int32_t X_D,  int32_t X_Max,  int32_t X_Min,
               int32_t Y_P,  int32_t Y_D,  int32_t Y_Max,  int32_t Y_Min);


#endif
