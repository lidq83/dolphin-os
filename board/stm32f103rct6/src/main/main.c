#include <typedef.h>

void RCC_config()
{
	ErrorStatus HSEStartUpStatus;

	RCC_DeInit();

	RCC_HSEConfig(RCC_HSE_ON);

	HSEStartUpStatus = RCC_WaitForHSEStartUp();

	if (HSEStartUpStatus == SUCCESS)
	{
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		RCC_PCLK1Config(RCC_HCLK_Div2);
		RCC_PCLK2Config(RCC_HCLK_Div1);
		RCC_PLLConfig(RCC_PLLSource_HSE_Div2, RCC_PLLMul_9);

		RCC_PLLCmd(ENABLE);
		while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
		{
		}

		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		while (RCC_GetSYSCLKSource() != 0x08)
		{
		}
	}
}

int main(int argc, char *argv[])
{
	SystemInit();

	RCC_config();

	while (1)
	{
	}
}
