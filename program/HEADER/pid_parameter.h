#ifndef __PID_PARAMETER_H
#define __PID_PARAMETER_H

typedef struct
{
	
	int32_t P;
    int32_t I;   //���һ��Ҳ���ã�0
    int32_t D;

    int32_t Out_Max;      //����޷�
    int32_t Out_Min;
	
////�����ĸ��̶��ģ�����1000 0 30 20
    int32_t Iteg_Max;     //�����޷�
    int32_t Dead_Zone;    //����
    int32_t Iteg_A;       //�������ٻ���A
    int32_t Iteg_B;       //�������ٻ���B
	
}PID_Parameter_Def;

typedef struct
{
	
    int32_t Set_Value;       //����ֵ
    int32_t Real_Value[2];   //���ֵ(ʵ��ֵ)
    int32_t Err[2];          //ƫ��
    int32_t PID_Output;      //PID������
    int32_t Err_Iteg;        //������
	
}PID_Cal_Def;

typedef struct
{
	
    PID_Parameter_Def Parameter;
    PID_Cal_Def Calculate;
	
}PID_Loop_Def;

typedef struct      //PID������
{

    PID_Loop_Def PID_Angle;
	PID_Loop_Def PID_X;
    PID_Loop_Def PID_Y;

}PID_Def;   //PID������

enum now_last
{
	
    Now = 1,
    Last = 0,
	
};//����ʱ�õ�

extern PID_Def Global_PID_Parameter;//��������Extern��ʱ��

#endif
