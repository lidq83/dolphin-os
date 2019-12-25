#include <tim3.h>
#include <led.h>

static u16 pwm_in = 0;
static u16 pwm_rise = 0;
static u16 pwm_drop = 0;

static uint8_t st = 0;

void tim3_in_init(void)
{
	tim3_irq_init();

	tim3_gpio_init();

	tim3_capture_init();
}

void tim3_irq_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU | GPIO_Mode_IPD;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource6);

	EXTI_InitStructure.EXTI_Line = EXTI_Line6;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void tim3_gpio_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_ResetBits(GPIOA, GPIO_Pin_6);
}

void tim3_capture_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM3_ICInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	//初始化定时器4 TIM3
	TIM_TimeBaseStructure.TIM_Period = 65535 - 1;
	TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_ICPSC_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

	TIM_Cmd(TIM3, DISABLE);
}

void EXTI9_5_IRQHandler(void)
{
	if (EXTI_GetITStatus(EXTI_Line6) != RESET)
	{
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == Bit_SET)
		{
			TIM_Cmd(TIM3, ENABLE);
		}
		else if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == Bit_RESET)
		{
			TIM_Cmd(TIM3, DISABLE);
			if (st == 1)
			{
				st = 0;
				pwm_in = TIM_GetCounter(TIM3);
			}
		}
		TIM_SetCounter(TIM3, 0);
	}
	EXTI_ClearITPendingBit(EXTI_Line6);
}

void tim3_set_st(uint8_t status)
{
	st = status;
}

uint16_t tim3_get_value()
{
	if (pwm_in > 26470)
	{
		return 26470;
	}
	return pwm_in;
}
