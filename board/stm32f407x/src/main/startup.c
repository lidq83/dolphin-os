#include <startup.h>

void RCC_WaitForPLLSturtUp(void)
{
	while ((RCC->CR & RCC_CR_PLLRDY) == 0)
	{
		__NOP();
	}
}

void rcc_config(void)
{
	RCC_DeInit();
	RCC_HSEConfig(RCC_HSE_ON);

	RCC_WaitForHSEStartUp();

	RCC_PLLConfig(RCC_PLLSource_HSE, 4, 336, 2, 2);
	RCC_PLLCmd(ENABLE);

	RCC_WaitForPLLSturtUp();

	RCC_HCLKConfig(RCC_SYSCLK_Div2);
	RCC_PCLK1Config(RCC_HCLK_Div1);
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
}