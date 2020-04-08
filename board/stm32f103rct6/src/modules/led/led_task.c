/*
 * task.c
 *
 *  Created on: December 23, 2019
 *      Author: lidq
 */

#include <led_task.h>

static led_s led = {0, 0x05};

static void led_pthread(void);

void led_pthread(void)
{
	for (uint8_t i = 0;; i++)
	{
		if ((led.led_val >> (i % 8)) & 0x1)
		{
			led_on(led.led_num);
		}
		else
		{
			led_off(led.led_num);
		}
		sleep_ticks(150);
	}
}

void led_task(void)
{
	pcb_create(28, &led_pthread, NULL, 512);
}
