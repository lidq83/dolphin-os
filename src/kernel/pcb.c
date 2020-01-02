/*
 * pcb.c
 *
 *  Created on: December 20, 2019
 *      Author: lidq
 */

#include <pcb.h>
#include <sche.h>

//记录优先级是0~255，第一个1的位置，也就是优先级位置
const uint8_t map_proi[256] = {
	/* 00 */ 0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* 10 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* 20 */ 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* 30 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* 40 */ 6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* 50 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* 60 */ 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* 70 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* 80 */ 7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* 90 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* A0 */ 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* B0 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* C0 */ 6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* D0 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* E0 */ 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* F0 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0
	/* END */
};

//进程就绪队列位图，bit位中为1表示已就绪，0表示未就绪
uint32_t pcb_ready_map = 0;

//内核中pcb，静态申请32个pcb
pcb_s pcbs[PROCESS_CNT] = {0};

//关中断
extern void sche_interrupt_disable(void);
//开中断
extern void sche_interrupt_enable(void);
//不保存现场切换任务
extern void PendSVHandler_nosave(void);

//当前正在运行的pcb
extern pcb_s *pcb_current;
//下一个需要运行的pcb
extern pcb_s *pcb_next;

//pcb运行函数
static void pcb_runner(void);

//初始化进程栈
void *stack_init(uint32_t *stack)
{
	uint32_t *stk = stack;

	// 初始化xPSP, PC, LR等寄存器
	*(--stk) = (uint32_t)0x01000000uL;
	*(--stk) = (uint32_t)pcb_runner;
	*(--stk) = (uint32_t)0x0uL;
	*(--stk) = (uint32_t)0x0uL;
	*(--stk) = (uint32_t)0x0uL;
	*(--stk) = (uint32_t)0x0uL;
	*(--stk) = (uint32_t)0x0uL;
	*(--stk) = (uint32_t)0x0uL;

	//初始化R11-R4
	*(--stk) = (uint32_t)0x0uL;
	*(--stk) = (uint32_t)0x0uL;
	*(--stk) = (uint32_t)0x0uL;
	*(--stk) = (uint32_t)0x0uL;
	*(--stk) = (uint32_t)0x0uL;
	*(--stk) = (uint32_t)0x0uL;
	*(--stk) = (uint32_t)0x0uL;
	*(--stk) = (uint32_t)0x0uL;

	return stk;
}

//创建一个进程
pcb_s *pcb_create(uint8_t prio, void *p_entry, void *p_arg, uint32_t stack_size)
{
	uint8_t *stack = malloc(stack_size);
	if (stack == NULL)
	{
		return NULL;
	}
	//初始化栈
	pcbs[prio].p_stack = stack_init(&stack[stack_size]);
	//栈内存地址
	pcbs[prio].p_stack_mem = stack;
	//优先级
	pcbs[prio].prio = prio;
	//休眠tick数
	pcbs[prio].sleep_tick = 0;
	//任务入口函数
	pcbs[prio].task_entry = p_entry;
	//入口函数参数
	pcbs[prio].task_arg = p_arg;
	//文件使用位图
	pcbs[prio].f_use_map = ~0x7;
	//初始化文件描述符
	memset(pcbs[prio].fnodes, 0, sizeof(void *) * FNODE_SIZE);
	//将空闲进程放入就绪队列
	pcb_ready(&pcbs[prio]);
	//返回pcb地址
	return &pcbs[prio];
}

void pcb_runner(void)
{
	//执行任务主函数
	pcb_current->task_entry(pcb_current->task_arg);
	//关中断
	sche_interrupt_disable();
	//挂起当前任务
	pcb_block(pcb_current);
	//释放栈内存
	free(pcb_current->p_stack_mem);
	//清空当前运行任务
	pcb_current = NULL;
	//取得下一个需要运行的pcb
	pcb_next = pcb_get_highest_pcb();
	//开中断
	//切换到其它需要运行的任务
	PendSVHandler_nosave();
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
pcb_s *pcb_get_highest_pcb(void)
{
	return &pcbs[pcb_get_highest_prio()];
}
