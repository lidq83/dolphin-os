#include <top.h>

extern pcb_s pcbs[PROCESS_CNT];
extern void sche_interrupt_disable(void);
extern void sche_interrupt_enable(void);

uint8_t top(top_s *t)
{
	if (t == NULL)
	{
		return 0;
	}

	uint8_t j = 0;
	//关中断
	sche_interrupt_disable();

	for (uint8_t i = 0; i < PROCESS_CNT; i++)
	{
		if (pcbs[i].stack_size > 0)
		{
			t[j].prio = i;
			t[j].stack_size = pcbs[i].stack_size;
			t[j].stack_used = t[j].stack_size;
			t[j].stack_free = 0;
			uint8_t *p = pcbs[i].p_stack_mem;
			for (uint32_t k = 0; k < pcbs[i].stack_size; k++)
			{
				if (p[k] == 0xff)
				{
					t[j].stack_free++;
					continue;
				}
				break;
			}
			t[j].stack_used = t[j].stack_size - t[j].stack_free;
			j++;
		}
	}

	//开中断
	sche_interrupt_enable();

	return j;
}