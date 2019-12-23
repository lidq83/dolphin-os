#include <tim4.h>

static uint32_t tick = 0;

static void timer4_run();

void timer4_run()
{
	tick++;
}

void timer4_init()
{
	//设置TIM类型
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	//设置TIM1时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	TIM_DeInit(TIM4);

	//自动重装载寄存器周期的值(计数值)
	TIM_TimeBaseStructure.TIM_Period = 1000 - 1;
	//时钟预分频数   例如：时钟频率=72MHZ/(时钟预分频+1)
	TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1);
	//采样分频
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	//向上计数模式
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	//清除溢出中断标志
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	//开启时钟
	TIM_Cmd(TIM4, ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void TIM4_IRQHandler()
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		timer4_run();
	}
	TIM_ClearITPendingBit(TIM4, TIM_FLAG_Update);
}
