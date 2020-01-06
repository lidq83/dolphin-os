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
#include <uart1.h>
#include <hw_config.h>

#define STACK_SIZE (512)

static void task_led(void);
static void task_0(void);
static void task_1(void);

static sem_t sem_rw = {0};

static led_s led = {0, 0x05};

void task_led(void)
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

void task_sem_post(void)
{
	uint32_t i = 0;
	while (1)
	{
		k_printf("task_0 post sem ,num = %d\n", i++);
		sem_post(&sem_rw);
		sleep_ticks(100);
	}
}

void task_sem_wait(void)
{
	uint32_t i = 0;
	while (1)
	{
		sem_wait(&sem_rw);
		k_printf("task_1 wait sem ,num = %d\n", i);
		if (i++ % 2 == 0)
		{
			led_on(1);
		}
		else
		{
			led_off(1);
		}
	}
}

void task_stdio(void)
{
	int a = 0;
	int b = 0;
	// char cmd[64] = {0};
	while (1)
	{
		k_printf("int a = ?, b = ?\n");
		k_scanf("%d,%d", &a, &b);
		k_printf("a + b = %d\n", a + b);
		k_printf("a * b = %d\n\n", a * b);

		// k_scanf("%s",cmd);
		// k_printf("%s\n", cmd);
	}
}

void task_usb(void)
{
	uint8_t buff[64] = {
		0,
		1,
		2,
		3,
		4,
		5,
		6,
		7,
		8,
		9,
	};
	uint32_t len = 0;
	while (1)
	{
		len = USB_RxRead(buff, 128);
		if (len > 0)
		{
			k_printf("read data len %d\n", len);
			USB_TxWrite(buff, len);
			//
		}
		USB_TxWrite(buff, 10);
		sleep_ticks(100);
	}
}

void task_led_blink(void)
{
	sem_init(&sem_rw, 0);

	//led闪烁
	pcb_create(PRIO_TASK_LED, &task_led, NULL, STACK_SIZE);

	//信号量示例
	// pcb_create(PRIO_TASK_SEM_POST, &task_sem_post, NULL, STACK_SIZE);
	// pcb_create(PRIO_TASK_SEM_WAIT, &task_sem_wait, NULL, STACK_SIZE);

	//标准输入输出示例
	pcb_create(PRIO_TASK_STDIO, &task_stdio, NULL, STACK_SIZE);

	//USB虚拟串口收发示例
	pcb_create(PRIO_TASK_USB, &task_usb, NULL, 1024);
}
