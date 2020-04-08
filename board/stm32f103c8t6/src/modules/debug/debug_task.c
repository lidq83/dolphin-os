/*
 * task.c
 *
 *  Created on: December 23, 2019
 *      Author: lidq
 */

#include <debug_task.h>

static void debug_pthread(void);

void debug_pthread(void)
{
	int a = 0;
	while (1)
	{
		k_printf("int a = %d\n", a);
		a++;
		sleep_ticks(10);
	}
}

void debug_task(void)
{
	pcb_create(24, &debug_pthread, NULL, 2048);
}
