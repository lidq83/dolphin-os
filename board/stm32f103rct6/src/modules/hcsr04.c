#include <led.h>
#include <tim2.h>
#include <tim3.h>
#include <hcsr04.h>
#include <uart1.h>
#include <protocol.h>
#include <string.h>
#include <sche.h>
#include <pcb.h>

#define STACK_SIZE (512)

extern led_s led[2];

static void gpio_init(void);

static void trig_on(void);

static void trig_off(void);

static void hcsr04_run(void);

void hcsr04_init()
{
	gpio_init();

	pcb_create(PRIO_HCSR04, &hcsr04_run, NULL, STACK_SIZE);
}

void hcsr04_run()
{
	uint32_t tick = 0;

	float dis = 4.0f;
	float dis_last = 2.0f;
	float dis_filter = 0.1f;

	uint8_t p_out[P_BUFF_SIZE] = {0};
	uint32_t out_len = 0;
	uint16_t data_seq = 0;
	uint16_t data_type = P_TYPE_DISTANCE;
	uint16_t data_len = sizeof(uint32_t);

	while (1)
	{
		uint32_t diff = tim3_get_value();
		if (diff > 0)
		{
			float dur = diff / 1000000.0f;
			float distance = 340.0f * dur / 2.0f;
			if (distance > 4.0f)
			{
				distance = 4.0f;
			}
			dis = distance * dis_filter + dis_last * (1.0f - dis_filter);
			dis_last = dis;
		}

		trig_on();
		for (int i = 0; i < 72 * 50; i++)
		{
		}
		trig_off();
		tim3_set_st(1);

		protocol_pack(p_out, &out_len, (uint8_t *)&dis, data_seq, data_type, data_len);
		data_seq++;
		Uart1_PutString(p_out, out_len);

		led[0].led_val |= 0x1;

		sleep_ticks(50);
	}
}

void gpio_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void trig_on(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_5);
}

void trig_off(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_5);
}
