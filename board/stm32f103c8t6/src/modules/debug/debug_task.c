/*
 * task.c
 *
 *  Created on: December 23, 2019
 *      Author: lidq
 */

#include <debug_task.h>

void debug_pthread(void)
{
	float num = 0.0;
	while (1)
	{
		k_printf("debug test %.2f\n", num);
		num+=0.01;
		sleep_ticks(100);
	}
}

void debug_task(void)
{
	pcb_create(8, &debug_pthread, NULL, 2048);
}
