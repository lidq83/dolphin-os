/*
 * pcb.c
 *
 *  Created on: December 20, 2019
 *      Author: lidq
 */

#include <pcb.h>
#include <sche.h>

//记录优先级是0~255，第一个1的位置，也就是优先级位置
uint8_t map_proi[256] = {
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

//进程就绪队列位图，bit位中为1表示已就绪，0表示未就绪
uint32_t pcb_ready_map = 0;

//内核中pcb，静态申请32个pcb
pcb_s pcbs[PROCESS_CNT] = { 0 };

//初始化进程栈
void *stack_init(void *p_entry, void *p_arg, void *stack)
{
	uint32_t *stk = stack;

	// 初始化xPSP, PC, LR等寄存器
	*(--stk) = (uint32_t) 0x01000000uL;
	*(--stk) = (uint32_t) p_entry;
	*(--stk) = (uint32_t) 0; /* R14 (LR)                                           */
	*(--stk) = (uint32_t) 0x12121212uL;
	*(--stk) = (uint32_t) 0x03030303uL;
	*(--stk) = (uint32_t) 0x02020202uL;
	*(--stk) = (uint32_t) 0x01010101uL;
	*(--stk) = (uint32_t) p_arg;

	//初始化R11-R4
	*(--stk) = (uint32_t) 0x11111111uL;
	*(--stk) = (uint32_t) 0x10101010uL;
	*(--stk) = (uint32_t) 0x09090909uL;
	*(--stk) = (uint32_t) 0x08080808uL;
	*(--stk) = (uint32_t) 0x07070707uL;
	*(--stk) = (uint32_t) 0x06060606uL;
	*(--stk) = (uint32_t) 0x05050505uL;
	*(--stk) = (uint32_t) 0x04040404uL;

	return stk;
}

//创建一个进程
pcb_s *pcb_create(uint8_t prio, void *p_entry, void *p_arg, uint32_t *stack)
{
	//初始化栈
	pcbs[prio].p_stack = stack_init(p_entry, p_arg, stack);
	//优先级
	pcbs[prio].prio = prio;
	//休眠tick数
	pcbs[prio].sleep_tick = 0;
	//将空闲进程放入就绪队列
	pcb_ready(&pcbs[prio]);
	//返回pcb地址
	return &pcbs[prio];
}

//将进程加入就绪队列
void pcb_ready(pcb_s *pcb)
{
	pcb_ready_map |= 1 << pcb->prio;
}

//将进程由就绪队列挂起
void pcb_block(pcb_s *pcb)
{
	pcb_ready_map &= ~(1 << pcb->prio);
}

//获取优先级最高的进程索引
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

//获取优先级最高的进程
pcb_s* pcb_get_highest_pcb(void)
{
	return &pcbs[pcb_get_highest_prio()];
}

