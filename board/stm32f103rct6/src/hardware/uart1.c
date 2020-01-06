/*
 * uart.c
 *
 *  Created on: Jun 25, 2017
 *      Author: lidq
 */

#include <string.h>
#include <uart1.h>

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

void UART1_GPIO_Configuration(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void UART1_Configuration(void)
{
	USART_InitTypeDef usart1_init_struct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	USART_Cmd(USART1, ENABLE);
	usart1_init_struct.USART_BaudRate = 115200;
	usart1_init_struct.USART_WordLength = USART_WordLength_8b;
	usart1_init_struct.USART_StopBits = USART_StopBits_1;
	usart1_init_struct.USART_Parity = USART_Parity_No;
	usart1_init_struct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	usart1_init_struct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART1, &usart1_init_struct);
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	NVIC_EnableIRQ(USART1_IRQn);
}

u8 Uart1_PutChar(u8 ch)
{
	USART_SendData(USART1, (u8)ch);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
	{
	}
	return ch;
}

void Uart1_PutString(u8 *buf, u8 len)
{
	for (u8 i = 0; i < len; i++)
	{
		Uart1_PutChar(*buf++);
	}
}

void USART1_IRQHandler(void)
{
	if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		while ((USART1->SR & USART_SR_RXNE) == 0)
		{
		}
		u8 ch = USART_ReceiveData(USART1);
		if (startup)
		{
			buff_append(ch);
			stdin_post_sem();
			stddev_putchar(ch);
		}
	}
}

void uart1_init()
{
	UART1_GPIO_Configuration();
	UART1_Configuration();
}
