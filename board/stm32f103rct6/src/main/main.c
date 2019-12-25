/*
 * task.c
 *
 *  Created on: December 23, 2019
 *      Author: lidq
 */

#include <typedef.h>
#include <led.h>
#include <tim2.h>
#include <tim3.h>
#include <uart1.h>
#include <hcsr04.h>
#include <core.h>
#include <protocol.h>
#include <pwmout.h>
#include <sysclk.h>
#include <task.h>

static void rcc_config();

int main(int argc, char *argv[])
{
	SystemInit();

	rcc_config();

	led_init();

	kernel_startup();

	task_led_blink();

	uart1_init();

	tim2_pwm_init();

	tim3_in_init();

	hcsr04_init();

	pwmout_init();

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
