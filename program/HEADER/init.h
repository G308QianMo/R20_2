#ifndef __INIT_H
#define __INIT_H
/**
*@file    init.h
*@author	Liu CX
*@version 1.3
*@date    2019.12.01
*@brief   进行硬件初始化配置
**/


void Init(void);
void GPIO_INIT(void);//IO口初始化
void Periph_Init(void);//外设初始化
void Driver_Init(void);//驱动初始化
void IMU_Init(void);//IMU初始化


//************************Debuger************************//
void Usart_SendByte( USART_TypeDef * pUSARTx, uint8_t ch);	//任意串口发送一个字符


//***********************外设使能控制************************//
#define 	USE_LED			1		//0=禁用,1=使用
#define 	USE_BEEP		1		//0=禁用,1=使用
#define 	USE_KEY			1		//0=禁用,1=使用
#define		USE_CAN_1		1		//0=禁用,1=使用
#define		USE_CAN_2		0		//0=禁用,1=使用
#define 	USE_USART_1		1		//0=禁用,1=使用
#define 	USE_USART_2		1		//0=禁用,1=使用
#define 	USE_USART_3		1	//0=禁用,1=使用
#define 	USE_UART_4		0		//0=禁用,1=使用
#define 	USE_UART_5		0		//0=禁用,1=使用
#define 	USE_USART_6		0		//0=禁用,1=使用
#define		USE_TIM_1		2		//0=禁用,1=定时器中断，2=编码器
#define		USE_TIM_2		2		//0=禁用,1=定时器中断，2=编码器
#define		USE_TIM_3		0		//0=禁用,1=定时器中断，2=编码器
#define		USE_TIM_4		0		//0=禁用,1=定时器中断，2=pwm输出
#define		USE_TIM_5		0		//0=禁用,1=定时器中断，2=编码器
#define		USE_TIM_6		0		//0=禁用,1=定时器中断
#define		USE_TIM_7		0		//0=禁用,1=定时器中断
#define		USE_TIM_8		0		//0=禁用,1=定时器中断




//系统初始化SystemInit函数里初始化APB1总线时钟为4分频即42M，
//所以TIM2~TIM7、TIM12~TIM14的时钟为APB1的时钟的两倍即84M；
//APB2总线时钟为2分频即84M，TIM1、TIM8~TIM11的时钟为APB2时钟的两倍即168M。
//定时周期T=(Period+1)*(Prescaler+1)/TCLK=200us

#if USE_LED
//************************LED************************//
#define LED0_ON GPIO_ResetBits(GPIOC, GPIO_Pin_0)	//开灯
#define LED1_ON GPIO_ResetBits(GPIOC, GPIO_Pin_1)
#define LED2_ON GPIO_ResetBits(GPIOC, GPIO_Pin_2)
#define LED3_ON GPIO_ResetBits(GPIOC, GPIO_Pin_3)

#define LED0_OFF GPIO_SetBits(GPIOC, GPIO_Pin_0)	//关灯
#define LED1_OFF GPIO_SetBits(GPIOC, GPIO_Pin_1)
#define LED2_OFF GPIO_SetBits(GPIOC, GPIO_Pin_2)
#define LED3_OFF GPIO_SetBits(GPIOC, GPIO_Pin_3)

#endif

#if USE_BEEP

//************************Beep************************//
#define BEEP_ON  GPIO_SetBits(GPIOA, GPIO_Pin_4)	//打开蜂鸣器
#define BEEP_OFF GPIO_ResetBits(GPIOA, GPIO_Pin_4)	//关闭蜂鸣器
void Beep_ms(uint16_t ms); 	//蜂鸣器鸣叫ms
#endif

#if USE_KEY
//************************KEY************************//
//返回值： 按下： (uint8_t)Bit_RESET
//		   松开： (uint8_t)Bit_SET
#define Read_KEY_1 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)
#define Read_KEY_2 GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_14)
#endif

#if USE_CAN_1
//************************CAN1************************//
#define		CAN1_RX_INT_ENABLE		1	//使能CAN1 FIFO0接收中断

#define		CAN1_SJW				CAN_SJW_1tq	
#define		CAN1_BS1				CAN_BS1_9tq
#define		CAN1_BS2				CAN_BS2_4tq
#define		CAN1_Prescaler			3		//分频系数

#define		CAN1_RX_FilterNum		0	//过滤器编号
#define		CAN1_FIFO				CAN_Filter_FIFO0
#define		CAN1_ReceiveInt			CAN_IT_FMP0		//中断源
#define		CAN1_RX_IRQ				CAN1_RX0_IRQn	
#define		CAN1_RX_IRQPrePri		1	//主优先级
#define		CAN1_RX_IRQSubPri		0	//次优先级
//#define		CAN1_RX_IRQHandler		CAN1_RX0_IRQHandler

#endif

#if USE_CAN_2
//************************CAN2************************//
#define		 CAN2_RX_INT_ENABLE		1	//使能CAN2 FIFO1接收中断

#define		CAN2_SJW				CAN_SJW_1tq	
#define		CAN2_BS1				CAN_BS1_9tq
#define		CAN2_BS2				CAN_BS2_4tq
#define		CAN2_Prescaler			3		//分频系数

#define		CAN2_RX_FilterNum		15	//过滤器编号
#define		CAN2_FIFO				CAN_Filter_FIFO1
#define		CAN2_ReceiveInt			CAN_IT_FMP1		//中断源		
#define		CAN2_RX_IRQ				CAN2_RX1_IRQn	
#define		CAN2_RX_IRQPrePri		1	//主优先级
#define		CAN2_RX_IRQSubPri		0	//次优先级
//#define		CAN2_RX_IRQHandler		CAN2_RX0_IRQHandler

#endif

#if USE_USART_1
//************************USART1************************//
//usart1由正点原子系统文件 usart.h搬移
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
#endif

#if USE_USART_2
//************************USART2************************//
#define 	USART2_BAUDRATE 		115200		//USART2波特率
#define		USART2_RX_ENABLE		1	//使能USART2接收中断

#define		USART2_RX_IRQPrePri		1	//主优先级
#define		USART2_RX_IRQSubPri		0	//次优先级
#endif

#if USE_USART_3
//************************USART3************************//
#define USART3_BAUDRATE 			115200		//USART3波特率
#define		USART3_RX_ENABLE		1	//使能USART3接收中断

#define		USART3_RX_IRQPrePri		1	//主优先级
#define		USART3_RX_IRQSubPri		0	//次优先级
#endif

#if USE_UART_4
//************************UART4************************//
#define 	UART4_BAUDRATE 			115200		//UART4波特率
#define		UART4_RX_ENABLE			1	//使能UART4接收中断

#define		UART4_RX_IRQPrePri		1	//主优先级
#define		UART4_RX_IRQSubPri		0	//次优先级
#endif

#if USE_UART_5
//************************UART5************************//
#define 	UART5_BAUDRATE 			115200		//UART5波特率
#define		UART5_RX_ENABLE			1	//使能UART5接收中断

#define		UART5_RX_IRQPrePri		1	//主优先级
#define		UART5_RX_IRQSubPri		1	//次优先级
#endif

#if USE_USART_6
//************************USART6************************//
#define 	USART6_BAUDRATE 		115200		//USART6波特率
#define		USART6_RX_ENABLE		1	//使能USART6接收中断

#define		USART6_RX_IRQPrePri		1	//主优先级
#define		USART6_RX_IRQSubPri		0	//次优先级
#endif

#if USE_TIM_1 == 1
//************************TIM1_Timer 168M ************************//
#define		TIM1_Period		500-1
#define		TIM1_Prescaler	1000-1	//分频
#define		TIM1_RepetitionCounter	168-1 //重复计数

#define		TIM1_IRQPrePri		1	//主优先级
#define		TIM1_IRQSubPri		0	//次优先级
#endif
#if USE_TIM_2 == 1
//************************TIM2_Timer 84M ************************//
#define		TIM2_Period		25000-1
#define		TIM2_Prescaler	1680-1

#define		TIM2_IRQPrePri		1	//主优先级
#define		TIM2_IRQSubPri		0	//次优先级
#endif

#if USE_TIM_3 == 1
//************************TIM3_Timer 84M ************************//
#define		TIM3_Period		25000-1
#define		TIM3_Prescaler	1680-1

#define		TIM3_IRQPrePri		1	//主优先级
#define		TIM3_IRQSubPri		0	//次优先级
#endif

#if USE_TIM_4 == 2
//************************TIM4_PWM************************//
#define		TIM4_Period		25000 - 1
#define		TIM4_Prescaler	1680 - 1
#endif

#if USE_TIM_4 == 1
//************************TIM4_Timer 84M ************************//
#define		TIM4_Period		25000-1
#define		TIM4_Prescaler	1680-1

#define		TIM4_IRQPrePri		1	//主优先级
#define		TIM4_IRQSubPri		0	//次优先级
#endif

#if USE_TIM_5 == 1
//************************TIM5_Timer 84M ************************//
#define		TIM5_Period		25000-1
#define		TIM5_Prescaler	1680-1

#define		TIM5_IRQPrePri		1	//主优先级
#define		TIM5_IRQSubPri		0	//次优先级
#endif

#if USE_TIM_6 == 1
//************************TIM6_Timer 84M ************************//
#define		TIM6_Period		25000-1
#define		TIM6_Prescaler	1680-1

#define		TIM6_IRQPrePri		1	//主优先级
#define		TIM6_IRQSubPri		0	//次优先级
#endif

#if USE_TIM_7 == 1
//************************TIM7_Timer 84M ************************//
#define		TIM7_Period		25000-1
#define		TIM7_Prescaler	1680-1

#define		TIM7_IRQPrePri		1	//主优先级
#define		TIM7_IRQSubPri		0	//次优先级
#endif

#if USE_TIM_8 == 1
//************************TIM8_Timer 168M ************************//
#define		TIM8_Period		500-1
#define		TIM8_Prescaler	1000-1	//分频
#define		TIM8_RepetitionCounter	168-1 //重复计数

#define		TIM8_IRQPrePri		1	//主优先级
#define		TIM8_IRQSubPri		0	//次优先级
#endif

#endif
