#include <sysclk.h>
#include <sche.h>

void sysclk_init()
{
	if (SysTick_Config(72000))
	{
		while (1)
		{
		}
	}
	//关闭时钟
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;

	while (72000)
	{
	}

	//打开时钟
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

void SysTick_Handler()
{
	sche_tick();
}
