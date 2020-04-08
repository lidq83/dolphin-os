/*
 * task.c
 *
 *  Created on: December 23, 2019
 *      Author: lidq
 */

#include <virtual_uart_task.h>
#include <string.h>
#include <k_printf.h>

static void virtual_uart_pthread(void);

void virtual_uart_pthread(void)
{
	uint32_t ind = 0;
	char buff[32] = {0};

	while (1)
	{
		number_to_str(buff, ind, 10);
		int len = strlen(buff);
		virtual_uart_send_buff(buff, len);
		virtual_uart_send_char('\n');
		ind++;
		sleep_ticks(100);
	}
}

void virtual_uart_task(void)
{
	pcb_create(22, &virtual_uart_pthread, NULL, 2048);
}
