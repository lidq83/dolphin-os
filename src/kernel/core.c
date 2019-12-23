/*
 * kernel.c
 *
 *  Created on: December 20, 2019
 *      Author: lidq
 */

#include <core.h>
#include <pcb.h>
#include <sche.h>
#include <led.h>

//空闲进程栈
static uint8_t idle_stack[128] = { 0 };

//空闲进程
static void process_idle(void);

//空闲进程
void process_idle(void)
{
	static uint32_t idle_ind = 0;

	while (1)
	{
		idle_ind++;
		for (int i = 0; i < 1000000; ++i)
		{
		}
		led_on(0);
		for (int i = 0; i < 1000000; ++i)
		{
		}
		led_off(0);
	}
}

//启动内核程序
void kernel_startup(void)
{
	sche_init();

	//创建空闲进程，优先级为最低
	pcb_create(31, &process_idle, NULL, &idle_stack[128]);
	//设置SV中断优先级
	sche_pend_sv_proi();
}
