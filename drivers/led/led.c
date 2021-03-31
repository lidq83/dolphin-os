#include <led.h>

void led_init(brd_def_led_s* leds, int cnt)
{
	for (int i = 0; i < cnt; i++)
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

		GPIO_WriteBit(leds[i].GPIOx, leds[i].GPIO_Pin, 1);
	}
}

void led_off(int led)
{
	// switch (led)
	// {
	// case 0:
	// 	GPIO_WriteBit(GPIOA, GPIO_Pin_6, 1);
	// 	break;
	// case 1:
	// 	GPIO_WriteBit(GPIOA, GPIO_Pin_7, 1);
	// 	break;
	// default:
	// 	break;
	// }
}

void led_on(int led)
{
	// switch (led)
	// {
	// case 0:
	// 	GPIO_WriteBit(GPIOA, GPIO_Pin_6, 0);
	// 	break;
	// case 1:
	// 	GPIO_WriteBit(GPIOA, GPIO_Pin_7, 0);
	// 	break;
	// default:
	// 	break;
	// }
}
