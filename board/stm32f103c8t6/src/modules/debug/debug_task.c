/*
 * task.c
 *
 *  Created on: December 23, 2019
 *      Author: lidq
 */

#include <debug_task.h>
#include <serial.h>

void debug_pthread(void)
{
	uint8_t ch = 0;
	while (1)
	{
		while (serial_read(&ch) > 0)
		{
			serial_write(ch);
		}
		sleep_ticks(1);
	}
}

void debug_task(void)
{
	pcb_create(8, &debug_pthread, NULL, 2048);
}
