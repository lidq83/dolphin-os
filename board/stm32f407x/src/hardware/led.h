#ifndef __LED_H
#define __LED_H

#include <typedef.h>

typedef struct led_s
{
	uint8_t led_num;
	uint8_t led_val;
} led_s;

void led_init();

void led_on(int led);

void led_off(int led);

#endif
