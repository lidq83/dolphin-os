#include <led.h>
#include <tim2.h>
#include <tim3.h>
#include <pwmout.h>
#include <uart1.h>
#include <protocol.h>
#include <string.h>
#include <sche.h>
#include <pcb.h>

#define STACK_SIZE (512)

extern led_s led[2];

buff_s buff_fifo = {0};

static void pwmout_run(void);

void pwmout_init()
{
	pcb_create(PRIO_PWMOUT, &pwmout_run, NULL, STACK_SIZE);
}

void pwmout_run()
{
	uint8_t p_out[P_BUFF_SIZE] = {0};
	uint32_t out_len = 0;
	uint16_t data_seq = 0;
	uint16_t data_type = 0;

	uint16_t pwm_out[4] = {0};
	uint16_t recv_dd = 0;

	while (1)
	{
		if (protocol_unpack(p_out, &out_len, &data_seq, &data_type, &buff_fifo) == 0)
		{
			switch (data_type)
			{
			case P_TYPE_MOTION:
				if (out_len == sizeof(uint16_t) * 4)
				{
					memcpy(pwm_out, p_out, out_len);
					tim2_pwm_set(pwm_out);
					recv_dd = 0;
				}
				break;
			}
		}

		if (recv_dd < 0xffff)
		{
			recv_dd++;
		}

		if (recv_dd > 20)
		{
			memset(pwm_out, 0, sizeof(uint16_t) * 4);
			tim2_pwm_set(pwm_out);

			led[0].led_val &= ~0x4;
		}
		else
		{
			led[0].led_val |= 0x4;
		}
		

		

		sleep_ticks(50);
	}
}
