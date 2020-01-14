#include "include.h"
/**
*@file    interrupt.c
*@author  Liu CX
*@version 1.1
*@date    2019.12.01
*@brief   中断初始化和管理
**/

extern int32_t Stop_flag;
extern int32_t Global_Target_X,Global_Target_Y,Global_Target_Angle;

/**
*@function Interrupt_Enable
*@param    无
*@brief    进行中断初始化和管理
*@retval   无
**/
void Interrupt_Enable(void)
{
	Beep_ms(100);
}


/**
 *@function CAN1_RX0_IRQHandler
 *@param    none
 *@brief    CAN1中断服务函数，使用FIFO0
 *@retval   none
**/
#if USE_CAN_1
#if CAN1_RX_INT_ENABLE	//使能RX0中断	    
void CAN1_RX0_IRQHandler(void)
{
	CanRxMsg RxMessage;
	if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET)	//判断中断挂起
	{
		CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);	//收数据，这里注意要选择FIFO
		//*******************************中断服务函数开始******************************//
switch(RxMessage.StdId)
        {
            //0x201~0x208对应8个m3508电机及8个c620电调，电调id范围为1~8。
            //通过can1读取8个电机的转子机械角度（0-8191），转速（单位rpm），电流（单位mA），转子温度（单位℃）
            case 0x201:
                Rotor_Read_Now[0].Angle = ((u16)RxMessage.Data[0] << 8) | (u16)(RxMessage.Data[1]);
                Rotor_Read_Now[0].Speed = ((u16)RxMessage.Data[2] << 8) | (u16)(RxMessage.Data[3]);
                Rotor_Read_Now[0].Cur = ((u16)RxMessage.Data[4] << 8) | (u16)(RxMessage.Data[5]);
                Rotor_Read_Now[0].Temp  = RxMessage.Data[6];
                Rotor_Position_Read(0);
                break;

            case 0x202:
                Rotor_Read_Now[1].Angle = ((u16)RxMessage.Data[0] << 8) | (u16)(RxMessage.Data[1]);
                Rotor_Read_Now[1].Speed = ((u16)RxMessage.Data[2] << 8) | (u16)(RxMessage.Data[3]);
                Rotor_Read_Now[1].Cur = ((u16)RxMessage.Data[4] << 8) | (u16)(RxMessage.Data[5]);
                Rotor_Read_Now[1].Temp  = RxMessage.Data[6];
                Rotor_Position_Read(1);
                break;

            case 0x203:
                Rotor_Read_Now[2].Angle = ((u16)RxMessage.Data[0] << 8) | (u16)(RxMessage.Data[1]);
                Rotor_Read_Now[2].Speed = ((u16)RxMessage.Data[2] << 8) | (u16)(RxMessage.Data[3]);
                Rotor_Read_Now[2].Cur = ((u16)RxMessage.Data[4] << 8) | (u16)(RxMessage.Data[5]);
                Rotor_Read_Now[2].Temp  = RxMessage.Data[6];
                Rotor_Position_Read(2);
                break;

            case 0x204:
                Rotor_Read_Now[3].Angle = ((u16)RxMessage.Data[0] << 8) | (u16)(RxMessage.Data[1]);
                Rotor_Read_Now[3].Speed = ((u16)RxMessage.Data[2] << 8) | (u16)(RxMessage.Data[3]);
                Rotor_Read_Now[3].Cur = ((u16)RxMessage.Data[4] << 8) | (u16)(RxMessage.Data[5]);
                Rotor_Read_Now[3].Temp  = RxMessage.Data[6];
                Rotor_Position_Read(3);
                break;
            case 0x205:
                Rotor_Read_Now[4].Angle = ((u16)RxMessage.Data[0] << 8) | (u16)(RxMessage.Data[1]);
                Rotor_Read_Now[4].Speed = ((u16)RxMessage.Data[2] << 8) | (u16)(RxMessage.Data[3]);
                Rotor_Read_Now[4].Cur = ((u16)RxMessage.Data[4] << 8) | (u16)(RxMessage.Data[5]);
                Rotor_Read_Now[4].Temp  = RxMessage.Data[6];
                Rotor_Position_Read(4);
                break;

            case 0x206:
                Rotor_Read_Now[5].Angle = ((u16)RxMessage.Data[0] << 8) | (u16)(RxMessage.Data[1]);
                Rotor_Read_Now[5].Speed = ((u16)RxMessage.Data[2] << 8) | (u16)(RxMessage.Data[3]);
                Rotor_Read_Now[5].Cur = ((u16)RxMessage.Data[4] << 8) | (u16)(RxMessage.Data[5]);
                Rotor_Read_Now[5].Temp  = RxMessage.Data[6];
                Rotor_Position_Read(5);
                break;

            case 0x207:
                Rotor_Read_Now[6].Angle = ((u16)RxMessage.Data[0] << 8) | (u16)(RxMessage.Data[1]);
                Rotor_Read_Now[6].Speed = ((u16)RxMessage.Data[2] << 8) | (u16)(RxMessage.Data[3]);
                Rotor_Read_Now[6].Cur = ((u16)RxMessage.Data[4] << 8) | (u16)(RxMessage.Data[5]);
                Rotor_Read_Now[6].Temp  = RxMessage.Data[6];
                Rotor_Position_Read(6);
                break;

            case 0x208:
                Rotor_Read_Now[7].Angle = ((u16)RxMessage.Data[0] << 8) | (u16)(RxMessage.Data[1]);
                Rotor_Read_Now[7].Speed = ((u16)RxMessage.Data[2] << 8) | (u16)(RxMessage.Data[3]);
                Rotor_Read_Now[7].Cur = ((u16)RxMessage.Data[4] << 8) | (u16)(RxMessage.Data[5]);
                Rotor_Read_Now[7].Temp  = RxMessage.Data[6];
                Rotor_Position_Read(7);
                break;
			case 0x13:		             
				Stop_flag = 1;
			break;
			
            default:
                break;
        }
		//*******************************中断服务函数结束******************************//
	}
}
#endif
#endif

/**
 *@function CAN2_RX1_IRQHandler
 *@param    none
 *@brief    CAN2中断服务函数，使用FIFO1
 *@retval   none
**/
#if USE_CAN_2
#if CAN2_RX_INT_ENABLE	//使能RX0中断		
int LED_Flag = 0;
extern int count;
void CAN2_RX1_IRQHandler(void)
{

//	CanRxMsg RxMessage;
//	if (CAN_GetITStatus(CAN2, CAN_IT_FMP1) != RESET)	//判断中断挂起
//	{
//		CAN_Receive(CAN2, CAN_FIFO1, &RxMessage);	//收数据，这里注意要选择FIFO
		//*******************************中断服务函数开始******************************//.

        GYRO_Get_Location_Param();

		//*******************************中断服务函数结束******************************//
//	}
}
#endif
#endif


/**
 *@function USART1_IRQHandler
 *@param    none
 *@brief    USART1中断服务函数
 *@retval   none
**/
#if USE_USART_1
#if EN_USART1_RX
void USART1_IRQHandler(void)
{
	uint8_t ucTemp;
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		ucTemp = USART_ReceiveData(USART1);
		//*******************************中断服务函数开始******************************//
		ANO_DT_Data_Receive_Prepare((uint8_t)ucTemp);
		//*******************************中断服务函数结束******************************//
	}
}
#endif
#endif


/**
 *@function USART2_IRQHandler
 *@param    none
 *@brief    USART2中断服务函数
 *@retval   none
**/
#if USE_USART_2
#if USART2_RX_ENABLE

void USART2_IRQHandler(void)
{
	uint8_t ucTemp;
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		ucTemp = USART_ReceiveData(USART2);
		//*******************************中断服务函数开始******************************//
		Data_Recieving(ucTemp);
		//*******************************中断服务函数结束******************************// 
	}
}
#endif
#endif

/**
 *@function USART3_IRQHandler
 *@param    none
 *@brief    USART3中断服务函数
 *@retval   none
**/
#if USE_USART_3
#if USART3_RX_ENABLE
void USART3_IRQHandler(void)
{
	uint8_t ucTemp;
	if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		ucTemp = USART_ReceiveData(USART3);
		//*******************************中断服务函数开始******************************//
//			 LED0_ON;
//		   delay_ms(1000);
//  		 LED0_OFF;
//		   delay_ms(1000);
     Packet_Decode(ucTemp);
		 IMU_Get_Location_Param();
		//*******************************中断服务函数结束******************************//  
	}
}
#endif
#endif

/**
 *@function UART4_IRQHandler
 *@param    none
 *@brief    UART4中断服务函数
 *@retval   none
**/
#if USE_UART_4
#if UART4_RX_ENABLE
void UART4_IRQHandler(void)
{
	uint8_t ucTemp;
	if (USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)
	{
		ucTemp = USART_ReceiveData(UART4);
		//*******************************中断服务函数开始******************************//

		//*******************************中断服务函数结束******************************//   
	}
}
#endif
#endif

/**
 *@function UART5_IRQHandler
 *@param    none
 *@brief    UART5中断服务函数
 *@retval   none
**/
#if USE_UART_5
#if UART5_RX_ENABLE
void UART5_IRQHandler(void)
{
	uint8_t ucTemp;
	if (USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)
	{
		ucTemp = USART_ReceiveData(UART5);
		//*******************************中断服务函数开始******************************//

		//*******************************中断服务函数结束******************************//
	}
}
#endif
#endif

/**
 *@function USART6_IRQHandler
 *@param    none
 *@brief    USART6中断服务函数
 *@retval   none
**/
#if USE_USART_6
#if USART6_RX_ENABLE
void USART6_IRQHandler(void)
{
	uint8_t ucTemp;
	if (USART_GetITStatus(USART6, USART_IT_RXNE) != RESET)
	{
		ucTemp = USART_ReceiveData(USART6);
		//*******************************中断服务函数开始******************************//

		//*******************************中断服务函数结束******************************// 
	}
}
#endif
#endif


/**
 *@function TIM1_UP_TIM10_IRQHandler
 *@param    none
 *@brief    TIM1中断服务函数
 *@retval   none
**/
#if USE_TIM_1 == 1
void TIM1_UP_TIM10_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
	{
		//*******************************中断服务函数开始******************************//

		//*******************************中断服务函数结束******************************//
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	}
}
#endif

/**
 *@function TIM2_IRQHandler
 *@param    none
 *@brief    TIM2中断服务函数
 *@retval   none
**/
#if USE_TIM_2 == 1
void  TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		//*******************************中断服务函数开始******************************//

		//*******************************中断服务函数结束******************************//
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}
#endif

/**
 *@function TIM3_IRQHandler
 *@param    none
 *@brief    TIM3中断服务函数
 *@retval   none
**/
#if USE_TIM_3 == 1
void  TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		//*******************************中断服务函数开始******************************//

		//*******************************中断服务函数结束******************************//
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}
#endif

/**
 *@function TIM4_IRQHandler
 *@param    none
 *@brief    TIM4中断服务函数
 *@retval   none
**/
#if USE_TIM_4 == 1
void  TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		//*******************************中断服务函数开始******************************//

		//*******************************中断服务函数结束******************************//
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}
#endif

/**
 *@function TIM5_IRQHandler
 *@param    none
 *@brief    TIM5中断服务函数
 *@retval   none
**/
#if USE_TIM_5 == 1
void  TIM5_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
	{
		//*******************************中断服务函数开始******************************//

		//*******************************中断服务函数结束******************************//
		TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
	}
}
#endif

/**
 *@function TIM6_DAC_IRQHandler
 *@param    none
 *@brief    TIM6中断服务函数
 *@retval   none
**/
#if USE_TIM_6 == 1
void  TIM6_DAC_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
	{
		//*******************************中断服务函数开始******************************//

		//*******************************中断服务函数结束******************************//
		TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
	}
}
#endif

/**
 *@function TIM7_IRQHandler
 *@param    none
 *@brief    TIM7中断服务函数
 *@retval   none
**/
#if USE_TIM_7 == 1
void  TIM7_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
	{
		//*******************************中断服务函数开始******************************//

		//*******************************中断服务函数结束******************************//
		TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
	}
}
#endif

/**
 *@function TIM8_UP_TIM13_IRQHandler
 *@param    none
 *@brief    TIM8中断服务函数
 *@retval   none
**/
#if USE_TIM_8 == 1
void TIM8_UP_TIM13_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM8, TIM_IT_Update) != RESET)
	{
		//*******************************中断服务函数开始******************************//

		//*******************************中断服务函数结束******************************//
		TIM_ClearITPendingBit(TIM8, TIM_IT_Update);
	}
}
#endif

