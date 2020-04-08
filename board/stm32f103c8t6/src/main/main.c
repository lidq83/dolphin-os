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
#include <led_task.h>
#include <stddev.h>

/***************************************************************************************
 * 
 * libs/STM32_USB-FS-Device_Lib_V4.0.0/Libraries/CMSIS/Device/ST/STM32F10x/Include/stm32f10x.h
 * #define HSE_VALUE    ((uint32_t)8000000)
 * #define HSI_VALUE    ((uint32_t)8000000)
 *
 ***************************************************************************************/

int startup = 0;

static void rcc_config();

int main(int argc, char *argv[])
{
	SystemInit();

	led_init();

	uart1_init();

	kernel_startup();

	pcb_clear_process();

	led_task();

	sysclk_init();

	while (1)
	{
	}
}
