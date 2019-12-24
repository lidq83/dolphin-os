/*
 * task.c
 *
 *  Created on: December 23, 2019
 *      Author: lidq
 */

#include <task.h>

#define STACK_SIZE (256)

static void task_led(void *arg);
static void task_0(void);
static void task_1(void);

static uint8_t stack_led[2][STACK_SIZE];
static uint8_t stack_task[2][STACK_SIZE];

static sem_t sem_rw = {0};

typedef struct led_s
{
	uint8_t led_num;
	uint8_t led_val;
} led_s;

static led_s led[2] = {{0, 0x05}, {1, 0x15}};

void task_led(void *arg)
{
	led_s *led_val = (led_s *)arg;

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
		sleep_ticks(200);
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

void task_create_examples(void)
{
	sem_init(&sem_rw, 0);

	pcb_create(24, &task_led, (void *)&led[0], &stack_led[0][STACK_SIZE]);
	pcb_create(26, &task_led, (void *)&led[1], &stack_led[1][STACK_SIZE]);

	pcb_create(20, &task_0, NULL, &stack_task[0][STACK_SIZE]);
	pcb_create(18, &task_1, NULL, &stack_task[1][STACK_SIZE]);
}
