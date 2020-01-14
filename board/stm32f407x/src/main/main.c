/*
 * task.c
 *
 *  Created on: December 23, 2019
 *      Author: lidq
 */
#include <typedef.h>
#include <led.h>
#include <sysclk.h>
#include <startup.h>
#include <uart1.h>
#include <core.h>
#include <stddev.h>
#include <task.h>

int startup = 0;

int main(int argc, char *argv[])
{
	rcc_config();

	led_init();

	uart1_init();

	kernel_startup();

	stddev_init();

	pcb_clear_process();

	task_led_blink();

	sysclk_init();

	while (1)
	{
	}
}
