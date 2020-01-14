#include "include.h"

/**
*@file    pid_parameter
*@author  HYH
*@version 1.0
*@date    2020/1/4
*@brief   没啥用的,定一个Global
**/

PID_Def Global_PID_Parameter = 
{
	{//Angle
     {1000,0,10,1000,-1000,  800,0,30,20},//P I D Out_Max Out_Min 后面四个不变(参数Struct)
     {0,0,0,0,0} //(计算Struct)
    },
	
	{//X
     {1000,0,10,1000,-1000,  1000,0,30,20},//P I D Out_Max Out_Min 后面四个不变(参数Struct)
     {0,0,0,0,0} //(计算Struct)
    },
	
	{//Y
     {1000,0,10,1000,-1000,  1000,0,30,20},//P I D Out_Max Out_Min 后面四个不变(参数Struct)
     {0,0,0,0,0} //(计算Struct)
    }
};
