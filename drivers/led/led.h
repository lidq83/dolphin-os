#ifndef __LED_H
#define __LED_H

#include <board.h>

typedef struct brd_def_led_s
{
	uint32_t RCC_Periph;
	uint32_t GPIO_Pin;
	GPIO_TypeDef* GPIOx;
} brd_def_led_s;

typedef struct led_s
{
	uint8_t led_num;
	uint8_t led_val;
} led_s;

void led_init(brd_def_led_s *leds, int cnt);

void led_on(int led);

void led_off(int led);

#endif
