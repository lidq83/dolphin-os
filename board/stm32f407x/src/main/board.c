/*
 * board.c
 *
 *  Created on: December 23, 2019
 *      Author: lidq
 */
#include <board.h>
#include <led.h>

brd_def_led_s leds[LED_CNT] = {
	{
		//
		.RCC_Periph = RCC_AHB1Periph_GPIOA,
		.GPIO_Pin = GPIO_Pin_6,
		.GPIOx = GPIOA,
		.val = 0
		//
	},
	{
		//
		.RCC_Periph = RCC_AHB1Periph_GPIOA,
		.GPIO_Pin = GPIO_Pin_7,
		.GPIOx = GPIOA,
		.val = 0
		//
	}
};

int board_init(void)
{
	led_init();
}
