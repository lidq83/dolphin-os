/*
 * sche.c
 *
 *  Created on: December 20, 2019
 *      Author: lidq
 */

#include <sche.h>
#include <slist.h>

//当前正在运行的pcb
pcb_s *pcb_current = NULL;
//下一个需要运行的pcb
pcb_s *pcb_next = NULL;
//pcb的休眠队列
static slist_s sleep_list = {0};

extern pcb_s pcbs[PROCESS_CNT];
extern void sche_interrupt_disable(void);
extern void sche_interrupt_enable(void);
extern void sche_switch_first(void);
extern void sche_switch(void);

//唤醒进程
static void wakeup(void);

void sche_init(void)
{
	slist_init(&sleep_list);
}

//获取当前运行的pcb
pcb_s *sche_curr_pcb(void)
{
	return pcb_current;
}

//系统心跳
void sche_tick(void)
{
	//关中断
	sche_interrupt_disable();

	//唤醒休眠完成的进程
	wakeup();
	//获取最高优先级的进程
	pcb_next = pcb_get_highest_pcb();
	//如果最高优先级进程就是当前进程，则进行跳转返回
	if (pcb_current == pcb_next)
	{
		goto _out;
	}
	if (pcb_current == NULL)
	{
		sche_switch_first();
	}
	else
	{
		//触发调度
		sche_switch();
	}

_out:
	//开中断
	sche_interrupt_enable();
}

//主动按tick数休眠
void sleep_ticks(uint32_t tick)
{
	//如tick为0则不休眠
	if (tick == 0)
	{
		return;
	}

	//关中断
	sche_interrupt_disable();

	//取得当前运行的进程
	pcb_s *p_curr = sche_curr_pcb();
	//设置休眠tick数
	p_curr->sleep_tick = tick;
	//挂起当前进程
	pcb_block(p_curr);
	//将当前进程加入休眠队列
	slist_append(&sleep_list, p_curr, p_curr);

	pcb_next = pcb_get_highest_pcb();
	//如果最高优先级进程就是当前进程，则进行跳转返回
	if (pcb_current != pcb_next)
	{
		//触发调度
		sche_switch();
	}
	//开中断
	sche_interrupt_enable();
}

//唤醒进程
void wakeup(void)
{
	//取得休眠队列中的头节点
	slist_node_s **p = &sleep_list.head;
	//在队列中循环遍历
	while (*p != NULL)
	{
		//取得pcb
		pcb_s *pcb = (pcb_s *)((*p)->value);
		//每执行一次，tick减一
		pcb->sleep_tick--;
		//如果tick数已经为0,表示需要结束休眠
		if (pcb->sleep_tick == 0)
		{
			//将休眠结束的pcb移出休眠队列
			slist_remove(&sleep_list, p, NULL);
			//将休眠结束的pcb加入就绪队列
			pcb_ready(pcb);
		}
		else
		{
			p = &(*p)->next;
		}
	}
}
