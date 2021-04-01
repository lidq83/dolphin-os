/*
 * task.c
 *
 *  Created on: December 23, 2019
 *      Author: lidq
 */

#include <led_task.h>

static led_s led[LED_CNT] = { 0, 0x05, 1, 0xAA };

void* led_pthread(void *arg)
{
	for (uint8_t i = 0;; i++)
	{
		for (int j = 0; j < LED_CNT; j++)
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
		sleep_ticks(100);
	}
	return NULL;
}

void led_task(void)
{
	pcb_create(PRIO_LED, &led_pthread, NULL, 500);
}
