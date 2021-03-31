#include <led_drv.h>

brd_def_led_s leds[LED_CNT] = {
	{
		.RCC_Periph = RCC_AHB1Periph_GPIOA,
		.GPIO_Pin = GPIO_Pin_6,
		.GPIOx = GPIOA
	},
	{
		.RCC_Periph = RCC_AHB1Periph_GPIOA,
		.GPIO_Pin = GPIO_Pin_7,
		.GPIOx = GPIOA
	}
};

void led_drv_init(void)
{
	led_init(leds, LED_CNT);
}