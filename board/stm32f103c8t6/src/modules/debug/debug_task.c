/*
 * task.c
 *
 *  Created on: December 23, 2019
 *      Author: lidq
 */

#include <debug_task.h>
#include <buff_s.h>
#include <virtual_uart.h>

extern buff_s rx_buff;

static void debug_pthread(void);

void debug_pthread(void)
{
	char buff[BUFF_SIZE_UART] = {0};
	while (1)
	{
		int b_size = buff_size(&rx_buff);
		for (int i = 0; i < b_size; i++)
		{
			buff[i] = rx_buff.buff[rx_buff.foot];
			rx_buff.foot++;
			rx_buff.foot %= BUFF_SIZE_UART;
		}
		if (b_size > 0)
		{
			virtual_uart_send_buff(buff, b_size);
		}
		sleep_ticks(10);
	}
}

void debug_pthread2(void)
{
	char *cmd0 = "AT+GMR\r\n";
	//char *cmd0 = "AT+UART=921600,8,1,0,0\r\n";
	//char *cmd0 = "AT+UART=2304000,8,1,0,0\r\n";
	//char *cmd0 = "AT+UART=4608000,8,1,0,0\r\n";

	while (1)
	{
		sleep_ticks(5000);
		Uart1_PutString(cmd0, strlen(cmd0));
	}
}

void debug_task(void)
{
	pcb_create(24, &debug_pthread, NULL, 2048);
	pcb_create(22, &debug_pthread2, NULL, 2048);
}
