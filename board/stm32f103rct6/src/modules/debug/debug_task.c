/*
 * task.c
 *
 *  Created on: December 23, 2019
 *      Author: lidq
 */

#include <debug_task.h>
#include <ttyS1.h>
#include <ttyS2.h>

static void debug_pthread(void);

void debug_pthread(void)
{
	int fd1 = open("/dev/ttyS1", 0, 0);
	if (fd1 < 0)
	{
		k_printf("open /dev/ttyS1 error.\n");
		return;
	}

	int fd2 = open("/dev/ttyS2", 0, 0);
	if (fd2 < 0)
	{
		k_printf("open /dev/ttyS2 error.\n");
		return;
	}

	uint8_t *buff = malloc(BUFF_SIZE_UART);
	memset(buff, 0, BUFF_SIZE_UART);
	while (1)
	{
		int len = read(fd1, buff, BUFF_SIZE_UART);
		if (len > 0)
		{
			write(fd1, buff, len);
		}

		// len = read(fd2, buff, BUFF_SIZE_UART);
		// if (len > 0)
		// {
		// 	write(fd1, buff, len);
		// }
		sleep_ticks(1);
	}
}

void debug_task(void)
{
	pcb_create(8, &debug_pthread, NULL, 2048);
}
