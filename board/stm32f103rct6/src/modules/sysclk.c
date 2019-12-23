#include <sysclk.h>
#include <sche.h>

//初始化系统时钟
void sysclk_init()
{
	//72000分频
	if (SysTick_Config(72000))
	{
		while (1)
		{
		}
	}
	//关闭时钟
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;

	//等待1毫秒
	while (72000)
	{
	}

	//打开时钟
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;
}

//系统时钟中断，频率1000Hz
void SysTick_Handler()
{
	sche_tick();
}
