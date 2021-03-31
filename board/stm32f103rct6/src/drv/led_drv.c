#include <led_drv.h>

brd_def_led_s leds[LED_CNT] = {
	{
		.RCC_Periph = RCC_APB2Periph_GPIOC,
		.GPIO_Pin = GPIO_Pin_13,
		.GPIOx = GPIOC
	},
	{
		.RCC_Periph = RCC_APB2Periph_GPIOC,
		.GPIO_Pin = GPIO_Pin_14,
		.GPIOx = GPIOC
	}
};

void led_drv_init(void)
{
	led_init(leds, LED_CNT);
}