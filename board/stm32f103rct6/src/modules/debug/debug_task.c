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
	int b = 0;
	// char cmd[64] = {0};
	while (1)
	{
		k_printf("int a = ?, b = ?\n");
		k_scanf("%d,%d", &a, &b);
		k_printf("a + b = %d\n", a + b);
		k_printf("a * b = %d\n\n", a * b);

		// k_scanf("%s",cmd);
		// k_printf("%s\n", cmd);
	}
}

void debug_task(void)
{
	pcb_create(24, &debug_pthread, NULL, 2048);
}
