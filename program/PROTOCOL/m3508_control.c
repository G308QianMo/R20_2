#include "include.h"
PID_Cal_para Cur_PID_Cal[8],Velo_PID_Cal[8],Pos_PID_Cal[8];
Rotor_Read Rotor_Read_Now[8];
Rotor_Raw Rotor_Pos[8];
s32 Current[8];
int Limit[8]={15000,15000,15000,15000,15000,15000,15000,15000};
u16 pos0[8];
int16_t g_m3508CtrlFlag=0;//闭环方式选择标志位
/**
*@file    m3508_ctrl.c
*@author  Pan
*@version 1.0
*@date    2020.1.10
*@brief   3508电机配c620/610电调用can1进行完全闭环控制及信息接受
**/



//can发送一组数据(固定格式:ID为0X12,标准帧,数据帧)
//len:数据长度(最大为8)
//msg:数据指针,最大为8个字节.
//返回值:0,成功;
//		 其他,失败;

u8 CAN1_Send_Msg(u8* msg, u32 TX_STD_ID)
{
    u8 mbox;
    u16 i = 0;
    CanTxMsg TxMessage;

    TxMessage.StdId = TX_STD_ID;	 // 标准标识符为0
    TxMessage.IDE = CAN_ID_STD;    //标准帧
    TxMessage.RTR = CAN_RTR_DATA;  //数据帧
    TxMessage.DLC = 0x08;          //帧长度为8

    for(i = 0; i < 8; i++)
    {
        TxMessage.Data[i] = msg[i];				 // 第一帧信息
    }
    mbox = CAN_Transmit(CAN1, &TxMessage);
    i = 0;
    while ((CAN_TransmitStatus(CAN1, mbox) == CAN_TxStatus_Failed) && (i < 0XFFF))i++;	//等待发送结束
    if(i >= 0XFFF)return 1;
    return 0;
}


//can口接收数据查询
//buf:数据缓存区;
//返回值:0,无数据被收到;
//		 其他,接收的数据长度;
u8 CAN1_Receive_Msg(u8 *buf)
{
    u32 i;
    CanRxMsg RxMessage;
    if( CAN_MessagePending(CAN1, CAN_FIFO0) == 0)return 0;		//没有接收到数据,直接退出
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//读取数据
    for(i = 0; i < RxMessage.DLC; i++)
        buf[i] = RxMessage.Data[i];
    return RxMessage.DLC;
}
	
//void pidParaWrite(double P,double I,double D)
//{
//		Pos_PID_Cal[0].PID.Proportion = 0.05;
//		Pos_PID_Cal[0].PID.Integral   = 0;
//		Pos_PID_Cal[0].PID.Derivative = 1;
//}

void m3508_control_cur0_3(int16_t cur0,int16_t cur1,int16_t cur2,int16_t cur3) //电流值范围为-16384~16384,单位mA
{
	uint32_t m3508_stdid = 0x000;	
    CanTxMsg Tx_message2;

		m3508_stdid = 0x200;

    Tx_message2.Data[0] = (u8)((cur0&0xff00) >> 8);
    Tx_message2.Data[1] = (u8)(cur0&0x00ff);
	
    Tx_message2.Data[2] = (u8)((cur1&0xff00) >> 8);
    Tx_message2.Data[3] = (u8)(cur1&0x00ff);
	
    Tx_message2.Data[4] = (u8)((cur2&0xff00) >> 8);
    Tx_message2.Data[5] = (u8)(cur2&0x00ff);

    Tx_message2.Data[6] = (u8)((cur3&0xff00) >> 8);
    Tx_message2.Data[7] = (u8)(cur3&0x00ff);

	CAN1_Send_Msg(Tx_message2.Data,m3508_stdid);
}


void m3508_control_cur4_7(int16_t cur4,int16_t cur5,int16_t cur6,int16_t cur7) //电流值范围为-16384~16384
{
	uint32_t m3508_stdid = 0x000;	
    CanTxMsg Tx_message;

	m3508_stdid = 0x1FF;

    Tx_message.Data[0] = (u8)((cur4&0xff00) >> 8);
    Tx_message.Data[1] = (u8)(cur4&0x00ff);
	
    Tx_message.Data[2] = (u8)((cur5&0xff00) >> 8);
    Tx_message.Data[3] = (u8)(cur5&0x00ff);
	
    Tx_message.Data[4] = (u8)((cur6&0xff00) >> 8);
    Tx_message.Data[5] = (u8)(cur6&0x00ff);

    Tx_message.Data[6] = (u8)((cur7&0xff00) >> 8);
    Tx_message.Data[7] = (u8)(cur7&0x00ff);

    
	CAN1_Send_Msg(Tx_message.Data,m3508_stdid);
}


int flag[8]={0,0,0,0,0,0,0,0};

void Rotor_Position_Read(int i)
{   
    Rotor_Pos[i].now = Rotor_Read_Now[i].Angle;
    if(!flag[i])
    {
        Rotor_Pos[i].last = Rotor_Pos[i].now;
        pos0[i]=Rotor_Pos[i].now;
        flag[i]=1;
        Rotor_Pos[i].N=0;
    }

    if((Rotor_Pos[i].now - Rotor_Pos[i].last) > 3000)
    {
        Rotor_Pos[i].N--;
    }
    else if((Rotor_Pos[i].last - Rotor_Pos[i].now) > 3000)
    {
        Rotor_Pos[i].N++;
    }
    Rotor_Read_Now[i].Pos = Rotor_Pos[i].N * 8191 + Rotor_Pos[i].now-pos0[i];

    Rotor_Pos[i].last = Rotor_Pos[i].now;
}

/*******************************************************************************
* @FunctionName   : PID_Cal
* @Description    : PID_Cal
* @Param          : PID_Cal_para
* @Retval         : PID
*******************************************************************************/
int k=0;
s32 PosPID_Cal(PID_Cal_para * p)
{
    p->Error = p->SetPoint - p->NextPoint;	  
    p->SumError += p->Error;
		p->PrevError = p->Error;
    p->dError = p->PrevError - p->LastError;
    p->LastError = p->PrevError;
    if(p->Error<10&&p->Error>-10)   //死区
    {
        p->Error=0;
        p->dError=0;
        p->SumError=0;
    }
    if(p->SumError >= p->SumErrLimit)
    {
        p->SumError = p->SumErrLimit;
    }
    if(p->SumError <= -p->SumErrLimit)
    {
        p->SumError = -p->SumErrLimit;
    }

//		printf("p->Error = %d  p->SumError = %d  p->dError = %d  ",(s32)p->Error,(s32)p->SumError,(s32)p->dError);

    return (p->PID.Proportion * p->Error
            +p->PID.Integral   * p->SumError
            + p->PID.Derivative * p->dError);
}


double VeloPID_Cal(PID_Cal_para * p)
{
	double pwm;
    p->Error = p->SetPoint - p->NextPoint;
    p->SumError += p->Error;
		p->PrevError = p->Error;
    p->dError = p->PrevError - p->LastError;
    p->LastError = p->PrevError;
    if(p->Error<10&&p->Error>-10)   //死区
    {
        p->Error=0;
        p->dError=0;
        p->SumError=0;
    }
    if(p->SumError >= p->SumErrLimit)
    {
        p->SumError = p->SumErrLimit;
    }
    if(p->SumError <= -p->SumErrLimit)
    {
        p->SumError = -p->SumErrLimit;
    }
		pwm = p->pwm + (p->PID.Proportion * p->Error + p->PID.Integral   * p->SumError + p->PID.Derivative * p->dError);
		p->pwm = pwm;
    return pwm;
}


s32 Current_Limit(double a,int limit)
{
	if(a > limit)
	{
		a = limit;
	}
	else if(a < -limit)
	{
		a = -limit;
	}
	return (s32)a;	
}



void M3508_Pos_Control(int i)//第i（1~8）个电机速度位置双环,position为出轴角度
{
	Pos_PID_Cal[i-1].NextPoint = Rotor_Read_Now[i-1].Pos;
	if(1)//限位条件，目前缺省
    {
        Pos_PID_Cal[i-1].SetPoint = Pos_PID_Cal[i-1].mySetPoint;
    }		
    Velo_PID_Cal[i-1].SetPoint = PosPID_Cal(&Pos_PID_Cal[i-1]);
    
    Velo_PID_Cal[i-1].NextPoint = Rotor_Read_Now[i-1].Speed;			

	Current[i-1] = VeloPID_Cal(&Velo_PID_Cal[i-1]);			
    Current[i-1] = Current_Limit(Current[i-1],Limit[i-1]);
}

void M3508_Vel_Control(int i)//第i（1~8）个电机速度环
{    
    Velo_PID_Cal[i-1].NextPoint = Rotor_Read_Now[i-1].Speed;			
    if(1)//限速条件，目前缺省
    {
        Velo_PID_Cal[i-1].SetPoint = Velo_PID_Cal[i-1].mySetPoint;
    }
	  Current[i-1] = VeloPID_Cal(&Velo_PID_Cal[i-1]);			
    Current[i-1] = Current_Limit(Current[i-1],Limit[i-1]);
}
void M3508_Pos_Velo_Control(int i)//第i（1~8）个电机速度位置并环
{		
	Pos_PID_Cal[i-1].NextPoint = Rotor_Read_Now[i-1].Pos;
    if(1)//限位条件，目前缺省
    {
        Pos_PID_Cal[i-1].SetPoint = Pos_PID_Cal[i-1].mySetPoint;
    }			
    Velo_PID_Cal[i-1].SetPoint = PosPID_Cal(&Pos_PID_Cal[i-1]);
    if(abs(Velo_PID_Cal[i-1].SetPoint)>abs(Velo_PID_Cal[i-1].mySetPoint))
    {
        Velo_PID_Cal[i-1].SetPoint = Velo_PID_Cal[i-1].mySetPoint;
    }
    
    Velo_PID_Cal[i-1].NextPoint = Rotor_Read_Now[i-1].Speed;			

	  Current[i-1] = VeloPID_Cal(&Velo_PID_Cal[i-1]);			
    Current[i-1] = Current_Limit(Current[i-1],Limit[i-1]);
}


void M3508_Angle_Set(int32_t i,double angle)
{
    g_m3508CtrlFlag = 1;
    Pos_PID_Cal[i-1].mySetPoint = (angle*GEAR_RATIO_3508/360)*8191;
}
void M3508_Vel_Set(int32_t i,s32 Velocity)
{
    g_m3508CtrlFlag = 2;
    Velo_PID_Cal[i-1].mySetPoint = Velocity*GEAR_RATIO_3508;
}
void M3508_Pos_Velo_Set(int32_t i,s32 Velocity,double angle)
{
    g_m3508CtrlFlag = 3;
    Pos_PID_Cal[i-1].mySetPoint = (angle*GEAR_RATIO_3508/360)*8191;
    Velo_PID_Cal[i-1].mySetPoint = Velocity*GEAR_RATIO_3508;
}

