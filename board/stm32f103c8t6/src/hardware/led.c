#include <led.h>

void led_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	led_off(0);
}

void led_on(int led)
{
	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
}

void led_off(int led)
{
	GPIO_SetBits(GPIOC, GPIO_Pin_13);
}
