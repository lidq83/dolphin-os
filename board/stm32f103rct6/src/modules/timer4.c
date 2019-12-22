#include <timer4.h>

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

	TIM_TimeBaseStructure.TIM_Period = 1000 - 1; //自动重装载寄存器周期的值(计数值)
	//累计 TIM_Period个频率后产生一个更新或者中断
	TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1); //时钟预分频数   例如：时钟频率=72MHZ/(时钟预分频+1)
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //采样分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM4, TIM_FLAG_Update); //清除溢出中断标志
	//TIM_PrescalerConfig(TIM4,0x8C9F,TIM_PSCReloadMode_Immediate); //时钟分频系数36000，所以定时器时钟为2K
	//TIM_ARRPreloadConfig(TIM4, DISABLE);       //禁止ARR预装载缓冲器
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

	TIM_Cmd(TIM4, ENABLE); //开启时钟

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

