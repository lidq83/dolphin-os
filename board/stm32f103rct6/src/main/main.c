/*
 * task.c
 *
 *  Created on: December 23, 2019
 *      Author: lidq
 */

#include <typedef.h>
#include <led.h>
#include <uart1.h>
#include <core.h>
#include <sysclk.h>
#include <task.h>
#include <stddev.h>

static void rcc_config();

int main(int argc, char *argv[])
{
	SystemInit();

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

void rcc_config()
{
	ErrorStatus HSEStartUpStatus;

	RCC_DeInit();

	RCC_HSEConfig(RCC_HSE_ON);

	HSEStartUpStatus = RCC_WaitForHSEStartUp();

	if (HSEStartUpStatus == SUCCESS)
	{
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		RCC_PCLK1Config(RCC_HCLK_Div2);
		RCC_PCLK2Config(RCC_HCLK_Div1);

		// 16 / 2 * 9 = 72
		RCC_PLLConfig(RCC_PLLSource_HSE_Div2, RCC_PLLMul_9);

		RCC_PLLCmd(ENABLE);
		while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
		{
		}

		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		while (RCC_GetSYSCLKSource() != 0x08)
		{
		}
	}
}
