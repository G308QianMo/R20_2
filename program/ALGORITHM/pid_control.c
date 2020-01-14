#include "include.h"

/**
*@file    pid_control
*@author  HYH
*@version 1.0
*@date    2020/1/4
*@brief   PID控制
**/

extern EncodePointTypeDef global_gyro_location;  //获取当前位置

/**
*@function PID_Set_Value_Angle
*@param    Angle 角度目标值
*@brief    给定目标值
*@retval   NULL
**/
//下面三个一个用法，中间函数
void PID_Set_Value_Angle(int32_t Angle)
{
	Global_PID_Parameter.PID_Angle.Calculate.Set_Value = Angle;
}

void PID_Set_Value_X(int32_t X)
{
	Global_PID_Parameter.PID_X.Calculate.Set_Value = X;
}

void PID_Set_Value_Y(int32_t Y)
{
	Global_PID_Parameter.PID_Y.Calculate.Set_Value = Y;
}

int32_t Global_Target_X = 0,Global_Target_Y = 0,Global_Target_Angle = 0;//目标值

/**
*@function PID_Loop_X
*@param    NULL
*@brief    PID实现
*@retval   NULL
**/
//下面三个作用一样
void PID_Loop_X(void)
{
	int32_t PID_Output_X;
	
	PID_Set_Value_X(Global_Target_X);
	PID_Output_X = PID_Cal(&Global_PID_Parameter.PID_X,global_gyro_location.x);
	Move_To_Point_Set(1,0,global_gyro_location.angle,PID_Output_X);
}	

void PID_Loop_Y(void)
{
	int32_t PID_Output_Y;
	
	PID_Set_Value_Y(Global_Target_Y);
	PID_Output_Y = PID_Cal(&Global_PID_Parameter.PID_Y,global_gyro_location.y);
	Move_To_Point_Set(0,1,global_gyro_location.angle,PID_Output_Y);
}

void PID_Loop_angle(void)
{
	int32_t PID_Output_A;
	
	PID_Set_Value_Angle(Global_Target_Angle);
	PID_Output_A = PID_Cal(&Global_PID_Parameter.PID_Angle,global_gyro_location.angle);
	MOVE_As_Circle(PID_Output_A);
	
//	printf("OUT:%d\t A:%f\n",PID_Output_A,global_gyro_location.angle);
}

/**
*@function PID_Calculate
*@param    *PID_Loop  定义为PID_Loop_Def的结构体的地址
           Real_Value  码盘接收到的当前真实值
*@brief    PID计算
*@retval   PID输出
**/
int32_t PID_Cal(PID_Loop_Def * PID_Loop,int32_t Real_Value)
{
    int32_t UP = 0; 
    int32_t UI = 0; 
    int32_t UD = 0; 
    int32_t Iteg_F; 

    PID_Loop->Calculate.Real_Value[Now] = Real_Value;                       
    PID_Loop->Calculate.Err[Now] = PID_Loop->Calculate.Set_Value - PID_Loop->Calculate.Real_Value[Now];   
    
    if (my_abs(PID_Loop->Calculate.Err[Now]) < PID_Loop->Parameter.Dead_Zone)
        PID_Loop->Calculate.Err[Now] = 0;

    
    else if (my_abs(PID_Loop->Calculate.Err[Now]) < PID_Loop->Parameter.Iteg_B)
        Iteg_F = 1;
    else if (my_abs(PID_Loop->Calculate.Err[Now]) < (PID_Loop->Parameter.Iteg_A + PID_Loop->Parameter.Iteg_B))
        Iteg_F = (PID_Loop->Parameter.Iteg_A - my_abs(PID_Loop->Calculate.Err[Now]) + PID_Loop->Parameter.Iteg_B) / PID_Loop->Parameter.Iteg_A;
    else
        Iteg_F = 0;
    PID_Loop->Calculate.Err_Iteg += (PID_Loop->Calculate.Err[Now] * Iteg_F);        

    
    if (PID_Loop->Calculate.Err_Iteg > PID_Loop->Parameter.Iteg_Max)       
		PID_Loop->Calculate.Err_Iteg = PID_Loop->Parameter.Iteg_Max;
    else if (PID_Loop->Calculate.Err_Iteg < -PID_Loop->Parameter.Iteg_Max) 
		PID_Loop->Calculate.Err_Iteg = -PID_Loop->Parameter.Iteg_Max;

    
    UP = PID_Loop->Calculate.Err[Now] * PID_Loop->Parameter.P;
    UI = PID_Loop->Calculate.Err_Iteg * PID_Loop->Parameter.I;
    UD = (PID_Loop->Calculate.Real_Value[Last] - PID_Loop->Calculate.Real_Value[Now]) * PID_Loop->Parameter.D; 
 //   	printf("p:%d\ti:%d\td:%d\n",UP,UI,UD);
    PID_Loop->Calculate.PID_Output = UP / 100 + UI / 100 + UD/100;
		
    
    if (PID_Loop->Calculate.PID_Output > PID_Loop->Parameter.Out_Max)      
		PID_Loop->Calculate.PID_Output = PID_Loop->Parameter.Out_Max;
    else if (PID_Loop->Calculate.PID_Output < PID_Loop->Parameter.Out_Min) 
		PID_Loop->Calculate.PID_Output = PID_Loop->Parameter.Out_Min;

    PID_Loop->Calculate.Real_Value[Last] = PID_Loop->Calculate.Real_Value[Now];            
    PID_Loop->Calculate.Err[Last] = PID_Loop->Calculate.Err[Now];                          
//	printf("OUT:%d\n",PID_Loop->Calculate.PID_Output);
    return PID_Loop->Calculate.PID_Output;
	

}

/**
*@function PID_Setup
*@param    如字面理解，设置PID参数
*@brief    设置PID参数
           缺省的参数取pid_parameter.c里面看注解，都不用的，设置的默认值
*@retval   NULL
**/
void PID_Setup(int32_t Angle_P,  int32_t Angle_D,  int32_t Angle_Max,  int32_t Angle_Min,
	           int32_t X_P,  int32_t X_D,  int32_t X_Max,  int32_t X_Min,
               int32_t Y_P,  int32_t Y_D,  int32_t Y_Max,  int32_t Y_Min)
{
	Global_PID_Parameter.PID_Angle.Parameter.P = Angle_P;
	Global_PID_Parameter.PID_Angle.Parameter.D = Angle_D;
	Global_PID_Parameter.PID_Angle.Parameter.Out_Max = Angle_Max;
	Global_PID_Parameter.PID_Angle.Parameter.Out_Min = Angle_Min;

    Global_PID_Parameter.PID_X.Parameter.P = X_P;
	Global_PID_Parameter.PID_X.Parameter.D = X_D;
	Global_PID_Parameter.PID_X.Parameter.Out_Max = X_Max;
	Global_PID_Parameter.PID_X.Parameter.Out_Min = X_Min;

    Global_PID_Parameter.PID_Y.Parameter.P = Y_P;
	Global_PID_Parameter.PID_Y.Parameter.D = Y_D;
	Global_PID_Parameter.PID_Y.Parameter.Out_Max = Y_Max;
	Global_PID_Parameter.PID_Y.Parameter.Out_Min = Y_Min;	
}
