#include <typedef.h>
#include <core.h>
#include <sche.h>
#include <led.h>
#include <sysclk.h>

#define STACK_SIZE	(256)

static void rcc_config();

static void task0(void);
static void task1(void);

static uint8_t stack0[STACK_SIZE];
static uint8_t stack1[STACK_SIZE];

int main(int argc, char *argv[])
{
	SystemInit();

	rcc_config();

	kernel_startup();

	pcb_create(0, &task0, NULL, &stack0[STACK_SIZE]);
	pcb_create(1, &task1, NULL, &stack1[STACK_SIZE]);

	sysclk_init();

	while (1)
	{
	}
}

void task0(void)
{
	uint32_t i = 0;
	while (1)
	{
		if (i++ % 2 == 0)
		{
			led_on(0);
		}
		else
		{
			led_off(0);
		}
		sleep_ticks(100);
	}
}

void task1(void)
{
	uint32_t i = 0;
	while (1)
	{
		if (i++ % 2 == 0)
		{
			led_on(1);
		}
		else
		{
			led_off(1);
		}
		sleep_ticks(100);
	}
}

void rcc_config()
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