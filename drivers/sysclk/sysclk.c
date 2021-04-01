#include <sysclk.h>
#include <sche.h>

void sysclk_init(void)
{
	//144000分频
	if (SysTick_Config(144000))
	{
		while (1)
		{
		}
	}
	//关闭时钟
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
	//等待1毫秒
	for (int i = 0; i < 144000; i++)
	{
	}
	//打开时钟
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
	SysTick_Config(SystemCoreClock / 1000);
	NVIC_SetPriority(SysTick_IRQn, 0xFF);
}

void SysTick_Handler(void)
{
	sche_tick();
}
