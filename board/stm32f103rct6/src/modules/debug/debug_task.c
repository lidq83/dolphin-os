/*
 * task.c
 *
 *  Created on: December 23, 2019
 *      Author: lidq
 */

#include <debug_task.h>

void debug_pthread(void)
{
	uint32_t num = 0;
	while (1)
	{
		k_printf("debug test %d\n", num);
		num++;
		sleep_ticks(1000);
	}
}

void debug_task(void)
{
	pcb_create(8, &debug_pthread, NULL, 2048);
}
