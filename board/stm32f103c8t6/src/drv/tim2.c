#include <tim2.h>
#include <uart1.h>

char buff[64];
int recv_size = 0;
extern uint32_t rx_cnt;
extern buff_s u1_rx_buff;

void tim2_init()
{
	// TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	// NVIC_InitTypeDef NVIC_InitStructure;
	// RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	// TIM_TimeBaseStructure.TIM_Period = 1000 - 1;
	// TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;
	// TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	// TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	// TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	// TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	// TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	// TIM_ITConfig(TIM2, TIM_IT_Update | TIM_IT_Trigger, ENABLE);

	// NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	// NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	// NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	// NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	// NVIC_Init(&NVIC_InitStructure);
	// TIM_Cmd(TIM2, ENABLE);
}

void TIM2_IRQHandler(void)
{
	static uint32_t i = 0;
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{
		uart1_recv(buff, &recv_size);
		if (i++ % 3000 == 0)
		{
			uart1_send(&rx_cnt, sizeof(rx_cnt));
		}
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}