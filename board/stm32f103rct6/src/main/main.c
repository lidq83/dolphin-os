/*
 * task.c
 *
 *  Created on: December 23, 2019
 *      Author: lidq
 */

#include <typedef.h>
#include <led.h>
#include <core.h>
#include <sysclk.h>
#include <serial1.h>
#include <serial2.h>
#include <ttyS1.h>
#include <ttyS2.h>
#include <std.h>
#include <debug_task.h>
#include <led_task.h>
#include <virtual_uart.h>

/***************************************************************************************
 * 
 * libs/STM32_USB-FS-Device_Lib_V4.0.0/Libraries/CMSIS/Device/ST/STM32F10x/Include/stm32f10x.h
 * #define HSE_VALUE    ((uint32_t)8000000)
 * #define HSI_VALUE    ((uint32_t)8000000)
 *
 ***************************************************************************************/

static void rcc_config();

int main(int argc, char *argv[])
{
	SystemInit();

	rcc_config();

	led_init();

	serial1_init();
	serial2_init();
	virtual_uart_init();

	kernel_startup();

	std_init();
	ttyS1_init();
	ttyS2_init();

	led_task();
	debug_task();

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

		// 8 / 1 * 9 = 72
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

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
