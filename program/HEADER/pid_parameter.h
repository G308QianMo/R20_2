#ifndef __PID_PARAMETER_H
#define __PID_PARAMETER_H

typedef struct
{
	
	int32_t P;
    int32_t I;   //这个一般也不用，0
    int32_t D;

    int32_t Out_Max;      //输出限幅
    int32_t Out_Min;
	
////下面四个固定的，不用1000 0 30 20
    int32_t Iteg_Max;     //积分限幅
    int32_t Dead_Zone;    //死区
    int32_t Iteg_A;       //连续变速积分A
    int32_t Iteg_B;       //连续变速积分B
	
}PID_Parameter_Def;

typedef struct
{
	
    int32_t Set_Value;       //给定值
    int32_t Real_Value[2];   //输出值(实际值)
    int32_t Err[2];          //偏差
    int32_t PID_Output;      //PID运算结果
    int32_t Err_Iteg;        //误差积分
	
}PID_Cal_Def;

typedef struct
{
	
    PID_Parameter_Def Parameter;
    PID_Cal_Def Calculate;
	
}PID_Loop_Def;

typedef struct      //PID环参数
{

    PID_Loop_Def PID_Angle;
	PID_Loop_Def PID_X;
    PID_Loop_Def PID_Y;

}PID_Def;   //PID环参数

enum now_last
{
	
    Now = 1,
    Last = 0,
	
};//计算时用的

extern PID_Def Global_PID_Parameter;//方便忘记Extern的时候

#endif
