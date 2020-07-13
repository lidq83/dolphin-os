/*
 * task.c
 *
 *  Created on: December 23, 2019
 *      Author: lidq
 */

#include <top_task.h>
#include <top.h>


void top_pthread(void)
{
	top_s t[12] = {0};

	while (1)
	{
		int cnt = top(t);
		k_printf("PID\t[USED / STACK]\n");
		for (int i = 0; i < cnt; i++)
		{
			k_printf("%u\t%u / %u\n", t[i].prio, t[i].stack_used, t[i].stack_size);
			sleep_ticks(5);
		}
		k_printf("\n");

		sleep_ticks(1000);
	}
}

void top_task(void)
{
	pcb_create(28, &top_pthread, NULL, 600);
}
