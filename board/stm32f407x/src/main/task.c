/*
 * task.c
 *
 *  Created on: December 23, 2019
 *      Author: lidq
 */

#include <task.h>
#include <led.h>
#include <k_printf.h>
#include <k_scanf.h>

#define STACK_SIZE (512)

static void task_led(void);

static led_s led = {0, 0x05};

void task_led(void)
{
	for (uint8_t i = 0;; i++)
	{
		if ((led.led_val >> (i % 8)) & 0x1)
		{
			led_on(led.led_num);
			led_on(1);
			led_on(2);
			led_on(3);
		}
		else
		{
			led_off(led.led_num);
			led_off(1);
			led_off(2);
			led_off(3);
		}
		sleep_ticks(125);
	}
}

void task_led_blink(void)
{
	//led闪烁
	pcb_create(PRIO_TASK_LED, &task_led, NULL, STACK_SIZE);
}
