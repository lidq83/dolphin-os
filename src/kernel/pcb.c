/*
 * pcb.c
 *
 *  Created on: December 20, 2019
 *      Author: lidq
 */

#include <pcb.h>
#include <sche.h>

//记录优先级是0~255，第一个1的位置，也就是优先级位置
uint8_t map_proi[256] =
{
	/* 00 */0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* 10 */4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* 20 */5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* 30 */4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* 40 */6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* 50 */4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* 60 */5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* 70 */4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* 80 */7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* 90 */4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* A0 */5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* B0 */4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* C0 */6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* D0 */4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* E0 */5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* F0 */4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0
	/* END */
};

uint32_t pcb_ready_map = 0;

static pcb_s pcbs[PROCESS_CNT] = { 0 };

static void *stack_init(void *p_entry, void *p_arg, uint32_t *stack);

static uint32_t pcb_get_highest_prio(void);

void *stack_init(void *p_entry, void *p_arg, uint32_t *stack)
{
	uint32_t *stk = stack;

	// 初始化xPSP, PC, LR等寄存器
	*(--stk) = (uint32_t) 0x01000000uL;
	*(--stk) = (uint32_t) p_entry;
	*(--stk) = (uint32_t) 0x0uL;
	*(--stk) = (uint32_t) 0x0uL;
	*(--stk) = (uint32_t) 0x0uL;
	*(--stk) = (uint32_t) 0x0uL;
	*(--stk) = (uint32_t) 0x0uL;
	*(--stk) = (uint32_t) p_arg;

	//初始化R11-R4
	*(--stk) = (uint32_t) 0x0uL;
	*(--stk) = (uint32_t) 0x0uL;
	*(--stk) = (uint32_t) 0x0uL;
	*(--stk) = (uint32_t) 0x0uL;
	*(--stk) = (uint32_t) 0x0uL;
	*(--stk) = (uint32_t) 0x0uL;
	*(--stk) = (uint32_t) 0x0uL;
	*(--stk) = (uint32_t) 0x0uL;

	return stk;
}

pcb_s *pcb_create(uint8_t prio, void *p_entry, void *p_arg, uint32_t *stack)
{
	pcbs[prio].p_stack = stack_init(p_entry, p_arg, stack);
	pcbs[prio].prio = prio;
	pcbs[prio].sleep_tick = 0;
	pcbs[prio].status = 0;

	return &pcbs[prio];
}

void pcb_ready(pcb_s *pcb)
{
	pcb_ready_map |= 1 << pcb->prio;
	pcb->status = 1;
}

void pcb_block(pcb_s *pcb)
{
	pcb_ready_map &= ~(1 << pcb->prio);
	pcb->status = 2;
}

uint32_t pcb_get_highest_prio(void)
{
	if (pcb_ready_map & 0xff)
	{
		return map_proi[pcb_ready_map & 0x00ff];
	}

	if (pcb_ready_map & 0xff00)
	{
		return map_proi[(pcb_ready_map & 0xff00) >> 8] + 8;
	}

	if ((pcb_ready_map & 0xff0000))
	{
		return map_proi[(pcb_ready_map & 0xff0000) >> 16] + 16;
	}

	return map_proi[(pcb_ready_map & 0xff000000) >> 24] + 24;
}

pcb_s* pcb_get_highest_pcb(void)
{
	return &pcbs[pcb_get_highest_prio()];
}