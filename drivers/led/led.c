#include <led.h>

extern brd_def_led_s leds[LED_CNT];

void led_init(void)
{
	for (int i = 0; i < LED_CNT; i++)
	{
		GPIO_InitTypeDef GPIO_InitStructure = { 0 };

#if defined(STM32F10X_MD)
		RCC_APB2PeriphClockCmd(leds[i].RCC_Periph, ENABLE);
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;

#elif defined(STM32F40XX)
		RCC_AHB1PeriphClockCmd(leds[i].RCC_Periph, ENABLE);
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
#endif
		GPIO_InitStructure.GPIO_Pin = leds[i].GPIO_Pin;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(leds[i].GPIOx, &GPIO_InitStructure);

		led_on(i);
	}
}

void led_on(int led)
{
	leds[led].val = 0;
	GPIO_WriteBit(leds[led].GPIOx, leds[led].GPIO_Pin, leds[led].val);
}

void led_off(int led)
{
	leds[led].val = 1;
	GPIO_WriteBit(leds[led].GPIOx, leds[led].GPIO_Pin, leds[led].val);
}

void led_blink(int led)
{
	GPIO_WriteBit(leds[led].GPIOx, leds[led].GPIO_Pin, leds[led].val);
	leds[led].val = !leds[led].val;
}