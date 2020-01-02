/*
 * task.c
 *
 *  Created on: December 23, 2019
 *      Author: lidq
 */

#include <task.h>
#include <led.h>

#define STACK_SIZE (256)

static void task_led(void *arg);
static void task_0(void);
static void task_1(void);

static sem_t sem_rw = { 0 };

// static led_s led[2] = {{0, 0x05}, {1, 0x15}};
led_s led[2] = { { 0, 0x0 }, { 1, 0x0 } };

void task_led(void *arg)
{
	led_s *led_val = (led_s *) arg;

	for (uint8_t i = 0;; i++)
	{
		if ((led_val->led_val >> (i % 8)) & 0x1)
		{
			led_on(led_val->led_num);
		}
		else
		{
			led_off(led_val->led_num);
		}
		sleep_ticks(150);
	}
}

void task_0(void)
{
	while (1)
	{
		sem_post(&sem_rw);
		sleep_ticks(150);
	}
}

void task_1(void)
{
	uint8_t i = 0;
	while (1)
	{
		sem_wait(&sem_rw);
		if (i++ % 2 == 0)
		{
			led_on(2);
		}
		else
		{
			led_off(2);
		}
	}
}

void task_led_blink(void)
{
	sem_init(&sem_rw, 0);

	//led闪烁
	pcb_create(PRIO_TASK_0, &task_led, (void *) &led[0], STACK_SIZE);
	pcb_create(PRIO_TASK_1, &task_led, (void *) &led[1], STACK_SIZE);

	//信号量示例
	pcb_create(PRIO_TASK_2, &task_0, NULL, STACK_SIZE);
	pcb_create(PRIO_TASK_3, &task_1, NULL, STACK_SIZE);
}
