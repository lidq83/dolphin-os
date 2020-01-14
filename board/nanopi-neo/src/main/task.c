/*
 * task.c
 *
 *  Created on: December 23, 2019
 *      Author: lidq
 */

#include <stdint.h>
#include <task.h>

#define STACK_SIZE (512)

static void task_example0(void);

static void task_example1(void);

void task_example0(void)
{
	while (1)
	{
		//sleep_ticks(150);
	}
}

void task_example1(void)
{
	while (1)
	{
		//sleep_ticks(150);
	}
}

void task_examples(void)
{
	pcb_create(20, &task_example0, NULL, STACK_SIZE);
	pcb_create(22, &task_example1, NULL, STACK_SIZE);
}
