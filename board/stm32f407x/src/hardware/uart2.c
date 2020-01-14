/*
 * uart.c
 *
 *  Created on: Jun 25, 2017
 *      Author: lidq
 */

#include <string.h>
#include <uart2.h>

buff_s rx_buff = {0};

extern int startup;
extern void stdin_post_sem(void);
extern void stddev_putchar(char ch);

int buff_append(char ch)
{
	rx_buff.buff[rx_buff.head] = ch;
	rx_buff.head++;
	rx_buff.head %= BUFF_SIZE;
	if (rx_buff.head == rx_buff.foot)
	{
		rx_buff.foot++;
		rx_buff.foot %= BUFF_SIZE;
	}
	return 0;
}

int buff_size(void)
{
	int size = rx_buff.head - rx_buff.foot;
	if (size < 0)
	{
		size += BUFF_SIZE;
	}
	return size;
}

void uart2_init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);  //使能GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //使能USART2时钟

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);  //GPIOA9复用为USART2
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2); //GPIOA10复用为USART2

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//速度50MHz
	// GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//推挽复用输出
	// GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//初始化PA9，PA10

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			//复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//速度50MHz
	// GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//推挽复用输出
	// GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//上拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//初始化PA9，PA10

	USART_Cmd(USART2, ENABLE);

	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;								//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//收发模式
	USART_Init(USART2, &USART_InitStructure);

	

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); //开启相关中断

	NVIC_InitTypeDef NVIC_InitStructure;
	//USART2 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;		  //串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		  //子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);							  //根据指定的参数初始化VIC寄存器
}

u8 Uart2_PutChar(u8 ch)
{
	USART_SendData(USART2, (u8)ch);
	while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
	{
	}
	return ch;
}

void Uart2_PutString(u8 *buf, u8 len)
{
	for (u8 i = 0; i < len; i++)
	{
		Uart2_PutChar(*buf++);
	}
}

void USART2_IRQHandler(void)
{
	if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
		while ((USART2->SR & USART_SR_RXNE) == 0)
		{
		}
		u8 ch = USART_ReceiveData(USART2);
		if (startup)
		{
			buff_append(ch);
			stdin_post_sem();
			stddev_putchar(ch);
		}
	}
}

