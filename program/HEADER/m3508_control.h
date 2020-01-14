#ifndef __M3508_CONTROL_H
#define __M3508_CONTROL_H
#include "sys.h"
#define GEAR_RATIO_3508 19.2        //3508电机原始减速比
typedef struct
{
    u16 last;
    u16 now;
    s32 N;
} Rotor_Raw;


typedef struct
{
    u16 Angle;
    s16 Speed;
    s16 Cur;
    s16 Temp;
    s32 Pos;
} Rotor_Read;


typedef struct
{
    double Proportion;
    double Integral;
    double Derivative;
} PID_para;


typedef struct
{
    double LastError, PrevError;
    double Error, SumError, dError;
    double SumErrLimit;
	  s32 SetAngle;
    s32 SetPoint;  //计算目标值
    s32 mySetPoint;//设定目标值
    s32 NextPoint;
    double pwm;
    PID_para PID;
} PID_Cal_para;

typedef struct
{
	int Dirction;
	int Velocity;
	int Phase;
}Leg_Statement;

extern PID_Cal_para Cur_PID_Cal[8], Velo_PID_Cal[8], Pos_PID_Cal[8];
extern Rotor_Read Rotor_Read_Now[8];
extern Rotor_Raw Rotor_Pos[8];
extern s32 Current[8];

void Rotor_Position_Read(int i);

void m3508_control_cur0_3(int16_t cur0, int16_t cur1, int16_t cur2, int16_t cur3);
void m3508_control_cur4_7(int16_t cur4, int16_t cur5, int16_t cur6, int16_t cur7);

//void pidParaWrite(double P,double I,double D);


double CurPID_Cal(PID_Cal_para * p);
double VeloPID_Cal(PID_Cal_para * p);
s32 PosPID_Cal(PID_Cal_para * p);


s32 Current_Limit(double current,int limit);
s32 M3508_Selflock_Cur_Control(int i);
void M3508_Vel_Control(int i);
void M3508_Pos_Control(int i);
void M3508_Pos_Velo_Control(int i);
void M3508_Angle_Set(int32_t i,double angle);
void M3508_Vel_Set(int32_t i,s32 Velocity);
void M3508_Pos_Velo_Set(int32_t i,s32 Velocity,double angle);

#endif
