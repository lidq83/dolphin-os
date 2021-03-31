#include <virtual_uart.h>
#include <buff_s.h>

static buff_s tx_buff = {0};
static uint16_t tx_data = 0x0;
static uint8_t tx_ind = 0xff;

void virtual_uart_init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_SetBits(GPIOB, GPIO_Pin_12);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	TIM_TimeBaseStructure.TIM_Prescaler = 1 - 1;
	TIM_TimeBaseStructure.TIM_Period = 1875 - 1; // 1000000 / 38400 * 72 = 1875
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);
	TIM_ITConfig(TIM4, TIM_IT_Update | TIM_IT_Trigger, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x10;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	TIM_Cmd(TIM4, ENABLE);
}

void virtual_uart_send_char(char ch)
{
	buff_append(&tx_buff, ch);
}

void virtual_uart_send_buff(char *buff, int len)
{
	for (int i = 0; i < len; i++)
	{
		buff_append(&tx_buff, buff[i]);
	}
}

uint16_t get_curr_data(void)
{
	uint16_t data = 0xFC00;
	int b_size = buff_size(&tx_buff);
	if (b_size > 0)
	{
		data = tx_buff.buff[tx_buff.foot];
		data <<= 1;
		data |= 0x200;
		data |= 9 << 10;

		tx_buff.foot++;
		tx_buff.foot %= BUFF_SIZE;
	}

	return data;
}

void TIM4_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);

		if (tx_ind > 9)
		{
			uint16_t data = get_curr_data();
			tx_data = data & 0x3FF;
			tx_ind = data >> 10 & 0xff;
		}

		if (tx_ind > 9)
		{
			return;
		}

		if ((tx_data >> (9 - tx_ind)) & 1)
		{
			GPIO_SetBits(GPIOB, GPIO_Pin_12);
		}
		else
		{
			GPIO_ResetBits(GPIOB, GPIO_Pin_12);
		}
		tx_ind--;
	}
}
