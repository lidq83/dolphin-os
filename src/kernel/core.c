/*
 * kernel.c
 *
 *  Created on: December 20, 2019
 *      Author: lidq
 */

#include <core.h>
#include <pcb.h>
#include <sche.h>
#include <mm.h>

extern pcb_s *pcb_next;

extern void sche_switch_first(void);

//空闲进程
static void process_idle(void);

//空闲进程
void process_idle(void)
{
	static uint32_t idle_ind = 0;

	while (1)
	{
		idle_ind++;
	}
}

//启动内核程序
void kernel_startup(void)
{
	mm_init();

	sche_init();

	uint8_t *idle_stack = (uint8_t *) malloc(128);
	//创建空闲进程，优先级为最低
	pcb_s *pcb_idle = pcb_create(31, &process_idle, NULL, &idle_stack[128]);
	pcb_next = pcb_idle;
}
