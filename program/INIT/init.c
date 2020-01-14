#include "include.h"

/**
*@file    init.c
*@author	Liu CX
*@version 1.3
*@date    2019.12.01
*@brief   进行硬件初始化,谁要是敢乱改我的程序我会记仇的，哼！
**/

extern float e[3];
extern float global_imu_angle_initial_value;

/**
 *@function Init
 *@param    无
 *@brief    用于所有硬件初始化管理
 *@retval   无
**/
void Init(void)
{
	GPIO_INIT();
	Periph_Init();
	Driver_Init();
	IMU_Init();
	Beep_ms(1000);
}
/**
 *@function Driver_Init
 *@param    无
 *@brief    用于驱动器等外部硬件的初始化管理
 *@retval   无
**/
void Driver_Init(void)
{
	GYRO_Init();
	delay_ms(1000);
	ELMO_Init();
}
/**
 *@function IMU_Init
 *@param    无
 *@brief    用于九轴IMU的初始化，并得到初值
 *@retval   无
**/
void IMU_Init(void)
{
	delay_ms(500);
	imu_data_decode_init();
	delay_ms(5000);
	get_eular(e);
	global_imu_angle_initial_value = e[2];
}

/**
 *@function GPIO_INIT
 *@param    无
 *@brief    用于IO口初始化管理
 *@retval   无
**/
void GPIO_INIT(void)
{

	GPIO_InitTypeDef  GPIO_InitStructure;

#if USE_LED
	//************************LED_INIT************************//
	//LED: PC0 PC1 PC2 PC3
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIOE时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;//LED对应引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//不上拉也不下拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化

	GPIO_SetBits(GPIOC, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3);//GPIO设置高，灯灭
#endif

#if USE_BEEP
	//************************Beep_INIT************************//
	//Beep: PA4	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//BEEP对应引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//普通输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//不上拉也不下拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO

	GPIO_ResetBits(GPIOA, GPIO_Pin_4);	//GPIO置低，蜂鸣器关闭
#endif

#if USE_KEY
	//************************KEY_INIT************************//
	//KEY: PC13 PC14
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 | GPIO_Pin_13; //KEY1 KEY2对应引脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//浮空
	GPIO_Init(GPIOC, &GPIO_InitStructure);
#endif

#if USE_CAN_1
	//************************CAN1_INIT************************//
	//CAN1_RX: PA11 
	//CAN1_TX: PA12
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);	//使能PORTA时钟	                   											 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			//复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//初始化PA11,PA12

	//引脚复用映射配置
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource11, GPIO_AF_CAN1); 	//GPIOA11复用为CAN1
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource12, GPIO_AF_CAN1); 	//GPIOA12复用为CAN1
#endif

#if USE_CAN_2
	//************************CAN2_INIT************************//
	//CAN2_RX: PB12 
	//CAN2_TX: PB13
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);	//初始化GPIOB时钟

	//初始化GPIO
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			//复用功能
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//初始化PB12,PB13

	//引脚复用映射配置
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_CAN2); 	//GPIOB12复用为CAN2 RX
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_CAN2); 	//GPIOB13复用为CAN2 TX
#endif

#if USE_USART_2
	//************************USART2_INIT************************//
	//USART2_TX: PA2
	//USART2_RX: PA3

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);//使能USART2时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3; //PA2,PA3,复用功能,上拉输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA2，PA3
	//WDNMD
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2); //GPIOA2复用为USART2
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2); //GPIOA3复用为USART2
#endif

#if USE_USART_3
	//************************USART3_INIT************************//
	//USART3_TX: PB10
	//USART3_RX: PB11

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); //使能GPIOB时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);//使能USART3时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10; //GPIOB11和GPIOB10初始化
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOB, &GPIO_InitStructure); //初始化GPIOB11，和GPIOB10

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3); //GPIOB11复用为USART3
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3); //GPIOB10复用为USART3
#endif

#if USE_UART_4
	//************************UART4_INIT************************//
	//UART4_TX: PC10
	//UART4_RX: PC11

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); //使能GPIOC时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);//使能UART4时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10; //GPIOC11和GPIOC10初始化
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure); //初始化GPIOB11，和GPIOB10

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4); //GPIOC11复用为UART4
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_UART4); //GPIOC10复用为UART4
#endif

#if USE_UART_5
	//************************UART5_INIT************************//
	//UART5_TX: PC12
	//UART5_RX: PD2

	USART_DeInit(UART5);

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); //使能GPIOC时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); //使能GPIOD时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);//使能UART5时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //GPIOD2初始化
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOD, &GPIO_InitStructure); //初始化GPIO

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12; //GPIOC12初始化
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure); //初始化GPIO

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_UART5);  //GPIOD2复用为UART4 RX
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_UART5); //GPIOAC12复用为UART4 TX
#endif

#if USE_USART_6
	//************************USART6_INIT************************//
	//USART6_TX: PC6
	//USART6_RX: PC7

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); //使能GPIOC时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);//使能USART6时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7; //PC6,PC7,复用功能,上拉输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	//速度50MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP; //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure); //初始化GPIO

	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_USART6); //GPIOC6复用为USART6
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_USART6); //GPIOC7复用为USART6
#endif


#if USE_TIM_1 == 2
	//************************TIM1_Encoder************************//
	//TIM1_CH1: PE9
	//TIM1_CH2: PE11


	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 	//使能GPIOE时钟	

	GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_TIM1); //GPIOE9复用为定时器1
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1); //GPIOE11复用为定时器1

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

#endif

#if USE_TIM_2 == 2
	//************************TIM2_Encoder************************//
	//TIM2_CH1: PA5
	//TIM2_CH2: PB3


	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能GPIOA时钟	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	//使能GPIOB时钟	

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_TIM2); //GPIOA5复用为定时器2
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_TIM2); //GPIOB3复用为定时器2

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

#endif
#if USE_TIM_3 == 2
	//************************TIM3_Encoder************************//
	//TIM3_CH1: PA6
	//TIM3_CH2: PA7


	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能GPIOA时钟	

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM3); //GPIOA6复用为定时器3
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_TIM3); //GPIOA7复用为定时器3

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

#endif

#if USE_TIM_4 == 2
	//************************TIM4_PWM************************//
	//TIM4_CH2: PD13	OP1
	//TIM4_CH3: PD14	OP2
	//TIM4_CH4: PD15	OPPWM

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);/*开启相关的GPIO外设时钟*/

	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4)/* 定时器通道引脚复用 */;
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4)/* 定时器通道引脚复用 */;
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4)/* 定时器通道引脚复用 */;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
#endif

#if USE_TIM_5 == 2
	//************************TIM5_Encoder************************//
	//TIM5_CH1: PA0
	//TIM5_CH2: PA1	

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能GPIOA时钟	

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5); //GPIOA0复用为定时器5
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5); //GPIOA1复用为定时器5

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

#endif
}


/**
 *@function Periph_Init
 *@param    无
 *@brief    用于外设初始化管理
 *@retval   无
**/
void Periph_Init(void)
{
#if USE_CAN_1 | USE_CAN_2
	CAN_InitTypeDef        	CAN_InitStructure;
	CAN_FilterInitTypeDef  	CAN_FilterInitStructure;
#endif
#if USE_USART_2 | USE_USART_3 | USE_UART_4 | USE_UART_5 | USE_USART_6
	USART_InitTypeDef 		USART_InitStructure;
#endif
	NVIC_InitTypeDef  		NVIC_InitStructure;
#if USE_TIM_1 | USE_TIM_2 | USE_TIM_3 | USE_TIM_4 | USE_TIM_5 | USE_TIM_6 | USE_TIM_7 | USE_TIM_8
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
#endif
	//************************delay&NVIC************************//
	delay_init(168);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2

#if USE_CAN_1
	//************************CAN1_INIT************************//
	//CAN1_RX: PA11 
	//CAN1_TX: PA12
	//通讯速率1M
	//过滤器0，不拦截任何报文（掩码 0x000）
	//报文存储于FIFO0

	//开时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);	//使能CAN1时钟	

	//CAN单元设置，
	CAN_InitStructure.CAN_TTCM = DISABLE;		//非时间触发通信模式   
	CAN_InitStructure.CAN_ABOM = DISABLE;		//软件自动离线管理	  
	CAN_InitStructure.CAN_AWUM = DISABLE;		//睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
	CAN_InitStructure.CAN_NART = ENABLE;		//禁止报文自动传送 
	CAN_InitStructure.CAN_RFLM = DISABLE;		//报文不锁定,新的覆盖旧的  
	CAN_InitStructure.CAN_TXFP = DISABLE;		//优先级由报文标识符决定 
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;	 //模式设置为普通模式
	CAN_InitStructure.CAN_SJW = CAN1_SJW;			//重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位 CAN_SJW_1tq~CAN_SJW_4tq
	CAN_InitStructure.CAN_BS1 = CAN1_BS1; 			//Tbs1范围CAN_BS1_1tq ~CAN_BS1_16tq
	CAN_InitStructure.CAN_BS2 = CAN1_BS2;			//Tbs2范围CAN_BS2_1tq ~	CAN_BS2_8tq
	CAN_InitStructure.CAN_Prescaler = CAN1_Prescaler;  	//分频系数(Fdiv)为brp+1	
	CAN_Init(CAN1, &CAN_InitStructure);   		//初始化CAN1 

	//配置过滤器
	CAN_FilterInitStructure.CAN_FilterNumber = CAN1_RX_FilterNum;	  					//过滤器0
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;	//32位 
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;					//32位ID
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;				//标识符和掩码都需要左移三位，由寄存器分配决定
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;					//32位MASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;				//标识符和掩码都需要左移三位，由寄存器分配决定
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN1_FIFO;			//过滤器0关联到FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE; 					//激活过滤器0
	CAN_FilterInit(&CAN_FilterInitStructure);

#if CAN1_RX_INT_ENABLE

	CAN_ITConfig(CAN1, CAN1_ReceiveInt, ENABLE);//FIFO0消息挂号中断允许.		    

	NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX_IRQ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = CAN1_RX_IRQPrePri;     // 主优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = CAN1_RX_IRQSubPri;            // 次优先级为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif	//#if CAN1_RX_INT_ENABLE

#endif	//#if USE_CAN_1

#if USE_CAN_2
	//************************CAN2_INIT************************//
	//CAN2_RX: PB12 
	//CAN2_TX: PB13
	//通讯速率1M
	//过滤器15，不拦截任何报文（掩码 0x000）  **特别地：CAN2的过滤器必须从14开始
	//报文存储于FIFO1

	//开时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN2, ENABLE);	//使能CAN2时钟	

	//CAN单元设置	
	CAN_InitStructure.CAN_TTCM = DISABLE;		//非时间触发通信模式   
	CAN_InitStructure.CAN_ABOM = DISABLE;		//软件自动离线管理	  
	CAN_InitStructure.CAN_AWUM = DISABLE;		//睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
	CAN_InitStructure.CAN_NART = ENABLE;		//禁止报文自动传送 
	CAN_InitStructure.CAN_RFLM = DISABLE;		//报文不锁定,新的覆盖旧的  
	CAN_InitStructure.CAN_TXFP = DISABLE;		//优先级由报文标识符决定 
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;	 //模式设置为普通模式
	CAN_InitStructure.CAN_SJW = CAN2_SJW;			//重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位 CAN_SJW_1tq~CAN_SJW_4tq
	CAN_InitStructure.CAN_BS1 = CAN2_BS1; 			//Tbs1范围CAN_BS1_1tq ~CAN_BS1_16tq
	CAN_InitStructure.CAN_BS2 = CAN2_BS2;			//Tbs2范围CAN_BS2_1tq ~	CAN_BS2_8tq
	CAN_InitStructure.CAN_Prescaler = CAN2_Prescaler;  	//分频系数(Fdiv)为brp+1	
	CAN_Init(CAN2, &CAN_InitStructure);   		//初始化CAN2 

	//配置过滤器
	CAN_FilterInitStructure.CAN_FilterNumber = CAN2_RX_FilterNum;	  					//过滤器15,can2的过滤器必须在14以上
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;	//32位 
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;					//32位ID
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;					//32位MASK
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN2_FIFO;			//过滤器1关联到FIFO1
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE; 					//激活过滤器1
	CAN_FilterInit(&CAN_FilterInitStructure);

#if CAN2_RX_INT_ENABLE

	CAN_ITConfig(CAN2, CAN2_ReceiveInt, ENABLE);//FIFO0消息挂号中断允许.		    

	NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX_IRQ;  //对于FIFO1，要把RX0改为RX1，CAN1同理
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = CAN2_RX_IRQPrePri;     // 主优先级为1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = CAN2_RX_IRQSubPri;            // 次优先级为0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#endif	//#if CAN2_RX_INT_ENABLE

#endif	//#if USE_CAN_2

#if USE_USART_1
	//************************USART1_INIT************************//
	//USART1_TX: PA9
	//USART1_RX: PA10
	uart_init(115200);
#endif

#if USE_USART_2
	//************************USART2_INIT************************//
	//USART2_TX: PA2
	//USART2_RX: PA3
	//波特率：默认115200（宏定义于 USART2_BAUDRATE）
	USART_InitStructure.USART_BaudRate = USART2_BAUDRATE;//默认115200
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式;
	USART_Init(USART2, &USART_InitStructure); //初始化串口
	USART_Cmd(USART2, ENABLE);  //使能串口

#if USART2_RX_ENABLE
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart2 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART2_RX_IRQPrePri;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART2_RX_IRQSubPri;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
#endif	//#if USART2_RX_ENABLE

#endif	//#if USE_USART_2

#if USE_USART_3
	//************************USART3_INIT************************//
	//USART3_TX: PB10
	//USART3_RX: PB11
	//波特率：默认115200（宏定义于 USART3_BAUDRATE）
	USART_InitStructure.USART_BaudRate = USART3_BAUDRATE;//波特率一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART3, &USART_InitStructure); //初始化串口3
	USART_Cmd(USART3, ENABLE); //使能串口

#if USART3_RX_ENABLE
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启相关中断

	//Usart2 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART3_RX_IRQPrePri;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART3_RX_IRQSubPri;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
#endif	//#if USART3_RX_ENABLE

#endif

#if USE_UART_4
	//************************UART4_INIT************************//
	//UART4_TX: PC10
	//UART4_RX: PC11
	//波特率：默认115200（宏定义于 UART4_BAUDRATE）
	USART_InitStructure.USART_BaudRate = UART4_BAUDRATE;//波特率一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(UART4, &USART_InitStructure); //初始化串口3
	USART_Cmd(UART4, ENABLE);  	//使能串口

#if UART4_RX_ENABLE
	USART_ITConfig(UART4, USART_IT_RXNE, ENABLE);//开启相关中断

	//UART4 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = UART4_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = UART4_RX_IRQPrePri;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = UART4_RX_IRQSubPri;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
#endif	//#if UART4_RX_ENABLE

#endif

#if USE_UART_5
	//************************UART5_INIT************************//
	//UART5_TX: PB10
	//UART5_RX: PB11
	//波特率：默认115200（宏定义于 UART5_BAUDRATE）
	USART_InitStructure.USART_BaudRate = UART5_BAUDRATE;//波特率一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(UART5, &USART_InitStructure); //初始化串口5
	USART_Cmd(UART5, ENABLE);  	//使能串口

#if UART5_RX_ENABLE
	USART_ITConfig(UART5, USART_IT_RXNE, ENABLE);//开启相关中断

	//UART5 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = UART5_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = UART5_RX_IRQPrePri;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = UART5_RX_IRQSubPri;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器、
#endif	//#if UART5_RX_ENABLE

#endif

#if USE_USART_6
	//************************USART6_INIT************************//
	//USART6_TX: PC6
	//USART6_RX: PC7
	//波特率：默认115200（宏定义于 USART6_BAUDRATE）
	USART_InitStructure.USART_BaudRate = USART6_BAUDRATE;//波特率一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	USART_Init(USART6, &USART_InitStructure); //初始化串口3
	USART_Cmd(USART6, ENABLE);                   //使能串口

#if USART6_RX_ENABLE
	USART_ITConfig(USART6, USART_IT_RXNE, ENABLE);//开启相关中断

	//USART6 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn;//串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = USART6_RX_IRQPrePri;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = USART6_RX_IRQSubPri;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
#endif	//#if USART6_RX_ENABLE

#endif

#if USE_TIM_1 == 2
	//************************TIM1_Encoder************************//
	//TIM1_CH1: PE9
	//TIM1_CH2: PE11

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);  	//TIM1时钟使能  
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // 预分频器 
	TIM_TimeBaseStructure.TIM_Period = (u16)(65535); //设定计数器自动重装值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//选择时钟分频：不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM向上计数  
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	TIM_EncoderInterfaceConfig(TIM1, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM1, &TIM_ICInitStructure);
	TIM_ClearFlag(TIM1, TIM_FLAG_Update);//清除TIM的更新标志位
	TIM_ITConfig(TIM1, TIM_IT_Update, DISABLE);	//禁用溢出中断
	//Reset counter
	TIM_SetCounter(TIM1, 30000);
	TIM_Cmd(TIM1, ENABLE);
#endif
#if USE_TIM_1 == 1
	//************************TIM1_Timer************************//
	// 高级控制定时器时钟源TIMxCLK = HCLK=168MHz 
	// 设定定时器频率为=TIMxCLK/(TIM_Prescaler+1)=10000Hz

	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TIM1_IRQPrePri;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = TIM1_IRQSubPri;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	TIM_TimeBaseStructure.TIM_Period = TIM1_Period;	//当定时器从0计数到4999，即为5000次，为一个定时周期
	TIM_TimeBaseStructure.TIM_Prescaler = TIM1_Prescaler;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	// 采样时钟分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	// 计数方式
	TIM_TimeBaseStructure.TIM_RepetitionCounter = TIM1_RepetitionCounter;	// 重复计数器:重复0次，即计数两次才生成一个中断
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	TIM_ClearFlag(TIM1, TIM_FLAG_Update);// 清除定时器更新中断标志位
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);// 开启定时器更新中断
	TIM_Cmd(TIM1, ENABLE);	// 使能定时器
	delay_ms(20);
#endif

#if USE_TIM_2 == 2
	//************************TIM2_Encoder************************//
	//TIM2_CH1: PA5
	//TIM2_CH2: PB3

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);  	//TIM2时钟使能 
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // 预分频器 
	TIM_TimeBaseStructure.TIM_Period = (u16)(65535); //设定计数器自动重装值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//选择时钟分频：不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM向上计数  
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);//清除TIM的更新标志位
	TIM_ITConfig(TIM2, TIM_IT_Update, DISABLE);	//禁用溢出中断
	//Reset counter
	TIM_SetCounter(TIM2, 30000);
	TIM_Cmd(TIM2, ENABLE);
#endif
#if USE_TIM_2 == 1	
	//************************TIM2_Timer************************//
	// 通用控制定时器时钟源TIMxCLK = HCLK/2=84MHz 

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;// 设置中断来源
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TIM2_IRQPrePri;	// 设置抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = TIM2_IRQSubPri;	// 设置子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseStructure.TIM_Period = TIM2_Period;
	TIM_TimeBaseStructure.TIM_Prescaler = TIM2_Prescaler;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	// 计数方式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);	// 初始化定时器TIMx, x[1,8]

	TIM_ClearFlag(TIM2, TIM_FLAG_Update);// 清除定时器更新中断标志位
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);// 开启定时器更新中断
	TIM_Cmd(TIM2, ENABLE);	// 使能定时器
	delay_ms(20);
#endif

#if USE_TIM_3 == 2
	//************************TIM3_Encoder************************//
	//TIM3_CH1: PA6
	//TIM3_CH2: PA7

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);  	//TIM3时钟使能 
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // 预分频器 
	TIM_TimeBaseStructure.TIM_Period = (u16)(65535); //设定计数器自动重装值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//选择时钟分频：不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM向上计数  
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);//清除TIM的更新标志位
	TIM_ITConfig(TIM3, TIM_IT_Update, DISABLE);	//禁用溢出中断
	//Reset counter
	TIM_SetCounter(TIM3, 0x00);
	TIM_Cmd(TIM3, ENABLE);
#endif
#if USE_TIM_3 == 1	
	//************************TIM3_Timer************************//
	// 通用控制定时器时钟源TIMxCLK = HCLK/2=84MHz 

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;// 设置中断来源
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TIM3_IRQPrePri;	// 设置抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = TIM3_IRQSubPri;	// 设置子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	TIM_TimeBaseStructure.TIM_Period = TIM3_Period;
	TIM_TimeBaseStructure.TIM_Prescaler = TIM3_Prescaler;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	// 计数方式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);	// 初始化定时器TIMx, x[1,8]

	TIM_ClearFlag(TIM3, TIM_FLAG_Update);// 清除定时器更新中断标志位
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);// 开启定时器更新中断
	TIM_Cmd(TIM3, ENABLE);	// 使能定时器
	delay_ms(20);
#endif

#if USE_TIM_4 == 2
	//************************TIM4_PWM************************//
	//TIM4_CH2: PD13	OP1
	//TIM4_CH3: PD14	OP2
	//TIM4_CH4: PD15	OPPWM
	//TIM4_CH1未引出
	// 通用控制定时器时钟源TIMxCLK = HCLK/2=84MHz 

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);


	TIM_TimeBaseStructure.TIM_Period = TIM4_Period;
	TIM_TimeBaseStructure.TIM_Prescaler = TIM4_Prescaler;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	    //配置为PWM模式1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  	  //当定时器计数值小于CCR1_Val时为高电平

	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);	 //使能通道2
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);	  /*使能通道2重载*/
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);	 //使能通道3
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);	  /*使能通道3重载*/
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);	 //使能通道4
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);	  /*使能通道4重载*/

	TIM_Cmd(TIM4, ENABLE);	// 使能定时器	

#endif

#if USE_TIM_4 == 1	
	//************************TIM4_Timer************************//
	// 通用控制定时器时钟源TIMxCLK = HCLK/2=84MHz 

	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;// 设置中断来源
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TIM4_IRQPrePri;	// 设置抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = TIM4_IRQSubPri;	// 设置子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

	TIM_TimeBaseStructure.TIM_Period = TIM4_Period;
	TIM_TimeBaseStructure.TIM_Prescaler = TIM4_Prescaler;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	// 计数方式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);	// 初始化定时器TIMx, x[1,8]

	TIM_ClearFlag(TIM4, TIM_FLAG_Update);// 清除定时器更新中断标志位
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);// 开启定时器更新中断
	TIM_Cmd(TIM4, ENABLE);	// 使能定时器
	delay_ms(20);
#endif
#if USE_TIM_5 == 2
	//************************TIM5_Encoder************************//
	//TIM5_CH1: PA0
	//TIM5_CH2: PA1	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);  	//TIM5时钟使能 
	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0; // 预分频器 
	TIM_TimeBaseStructure.TIM_Period = (u16)(65535); //设定计数器自动重装值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;//选择时钟分频：不分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;////TIM向上计数  
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
	TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//使用编码器模式3
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM5, &TIM_ICInitStructure);
	TIM_ClearFlag(TIM5, TIM_FLAG_Update);//清除TIM的更新标志位
	TIM_ITConfig(TIM5, TIM_IT_Update, DISABLE);	//禁用溢出中断
	//Reset counter
	TIM_SetCounter(TIM5, 0x00);
	TIM_Cmd(TIM5, ENABLE);
#endif

#if USE_TIM_5 == 1	
	//************************TIM5_Timer************************//
	// 通用控制定时器时钟源TIMxCLK = HCLK/2=84MHz 

	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;// 设置中断来源
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TIM5_IRQPrePri;	// 设置抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = TIM5_IRQSubPri;	// 设置子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

	TIM_TimeBaseStructure.TIM_Period = TIM5_Period;
	TIM_TimeBaseStructure.TIM_Prescaler = TIM5_Prescaler;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	// 计数方式
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);	// 初始化定时器TIMx, x[1,8]

	TIM_ClearFlag(TIM5, TIM_FLAG_Update);// 清除定时器更新中断标志位
	TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);// 开启定时器更新中断
	TIM_Cmd(TIM5, ENABLE);	// 使能定时器
	delay_ms(20);
#endif

#if USE_TIM_6 == 1
	//************************TIM6_Timer************************//
	// 基本定时器时钟源TIMxCLK = HCLK/2=84MHz 

	NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;// 设置中断来源
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TIM6_IRQPrePri;	// 设置抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = TIM6_IRQSubPri;// 设置子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);	// 开启TIMx_CLK,x[6,7] 

	TIM_TimeBaseStructure.TIM_Period = TIM6_Period;//当定时器从0计数到4999，即为5000次，为一个定时周期
	TIM_TimeBaseStructure.TIM_Prescaler = TIM6_Prescaler;
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);// 初始化定时器TIMx, x[2,3,4,5]
	TIM_ClearFlag(TIM6, TIM_FLAG_Update);// 清除定时器更新中断标志位
	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE);// 开启定时器更新中断
	TIM_Cmd(TIM6, ENABLE);// 使能定时器
	delay_ms(20);
#endif

#if USE_TIM_7 == 1
	//************************TIM7_Timer************************//
	// 基本定时器时钟源TIMxCLK = HCLK/2=84MHz 

	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;// 设置中断来源
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TIM7_IRQPrePri;	// 设置抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = TIM7_IRQSubPri;// 设置子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);	// 开启TIMx_CLK,x[6,7] 

	TIM_TimeBaseStructure.TIM_Period = TIM7_Period;//当定时器从0计数到4999，即为5000次，为一个定时周期
	TIM_TimeBaseStructure.TIM_Prescaler = TIM7_Prescaler;
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);// 初始化定时器TIMx, x[2,3,4,5]
	TIM_ClearFlag(TIM7, TIM_FLAG_Update);// 清除定时器更新中断标志位
	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);// 开启定时器更新中断
	TIM_Cmd(TIM7, ENABLE);// 使能定时器
	delay_ms(20);
#endif

#if USE_TIM_8 == 1
	//************************TIM8_Timer************************//
	// 高级控制定时器时钟源TIMxCLK = HCLK=168MHz 
	// 设定定时器频率为=TIMxCLK/(TIM_Prescaler+1)=10000Hz

	NVIC_InitStructure.NVIC_IRQChannel = TIM8_UP_TIM13_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TIM8_IRQPrePri;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = TIM8_IRQSubPri;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);

	TIM_TimeBaseStructure.TIM_Period = TIM8_Period;	//当定时器从0计数到4999，即为5000次，为一个定时周期
	TIM_TimeBaseStructure.TIM_Prescaler = TIM8_Prescaler;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	// 采样时钟分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	// 计数方式
	TIM_TimeBaseStructure.TIM_RepetitionCounter = TIM8_RepetitionCounter;	// 重复计数器:重复0次，即计数两次才生成一个中断
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);

	TIM_ClearFlag(TIM8, TIM_FLAG_Update);// 清除定时器更新中断标志位
	TIM_ITConfig(TIM8, TIM_IT_Update, ENABLE);// 开启定时器更新中断
	TIM_Cmd(TIM8, ENABLE);	// 使能定时器
	delay_ms(20);

#endif

}

#if USE_BEEP
/**
 *@function Beep_ms
 *@param    uint16_t ms
 *@brief    蜂鸣器鸣叫ms
 *@retval   none`
**/
void Beep_ms(uint16_t ms)
{
	BEEP_ON;
	delay_ms(ms);
	BEEP_OFF;
}

#endif

/**
 *@function Usart_SendByte
 *@param    USART_TypeDef * pUSARTx  串口地址
			uint8_t ch	待发送字符
 *@brief    串口发送一个字符
 *@retval   none`
**/
void Usart_SendByte(USART_TypeDef * pUSARTx, uint8_t ch)
{
	/* 发送一个字节数据到USART */
	USART_SendData(pUSARTx, ch);

	/* 等待发送数据寄存器为空 */
	while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}


