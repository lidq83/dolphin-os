/*
 * task.c
 *
 *  Created on: December 23, 2019
 *      Author: lidq
 */
#include <typedef.h>
#include <led.h>
#include <sysclk.h>
#include <core.h>
#include <stddev.h>
#include <task.h>

int startup = 0;

int main(int argc, char *argv[])
{
	led_init();

	kernel_startup();

	stddev_init();

	pcb_clear_process();

	task_led_blink();

	startup = 1;

	sysclk_init();

	while (1)
	{
	}
}
