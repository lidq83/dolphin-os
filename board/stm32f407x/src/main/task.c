/*
 * task.c
 *
 *  Created on: December 23, 2019
 *      Author: lidq
 */

#include <k_printf.h>
#include <led.h>
#include <task.h>

#define STACK_SIZE (512)

static void task_led(void);

static led_s led[2] = { 0, 0x05, 1, 0xAA };

void task_led(void)
{
	for (uint8_t i = 0;; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			if ((led[j].led_val >> (i % 8)) & 0x1)
			{
				led_on(led[j].led_num);
			}
			else
			{
				led_off(led[j].led_num);
			}
		}
		sleep_ticks(125);
	}
}

void task_led_blink(void)
{
	//led闪烁
	pcb_create(PRIO_TASK_LED, &task_led, NULL, STACK_SIZE);
}
