#include <sysclk.h>
#include <sche.h>

void sysclk_init(void)
{
	SysTick_Config(SystemCoreClock / 1000);
	NVIC_SetPriority(SysTick_IRQn, 1);
}

void SysTick_Handler(void)
{
	sche_tick();
}
