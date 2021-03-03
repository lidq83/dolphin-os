#include <led.h>

void led_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	led_off(0);
	led_off(1);
}

void led_off(int led)
{
	switch (led)
	{
	case 0:
		GPIO_ResetBits(GPIOA, GPIO_Pin_6);
		break;
	case 1:
		GPIO_ResetBits(GPIOA, GPIO_Pin_7);
		break;
	default:
		break;
	}
}

void led_on(int led)
{
	switch (led)
	{
	case 0:
		GPIO_SetBits(GPIOA, GPIO_Pin_6);
		break;
	case 1:
		GPIO_SetBits(GPIOA, GPIO_Pin_7);
		break;
	default:
		break;
	}
}
