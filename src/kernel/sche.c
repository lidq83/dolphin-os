/*
 * sche.c
 *
 *  Created on: December 20, 2019
 *      Author: lidq
 */

#include <sche.h>
#include <slist.h>

//PendSV中断控制器地址
#define NVIC_INT_CTRL 0xE000Ed04
//触发PendSV
#define NVIC_PENDSV_SET 0x10000000
//PendSV优先级控制地址
#define NVIC_SYSPRI2 0xE000Ed22
//PendSV设置为最低优先值255
#define NVIC_PENDSV_PRI 0x000000ff

//当前正在运行的pcb
static pcb_s *p_current = NULL;
//下一个需要运行的pcb
static pcb_s *p_next = NULL;
//pcb的休眠队列
static slist_s sleep_list = {0};
//唤醒进程
static void wakeup(void);

//设置SV中断优先级
void sche_pend_sv_proi(void)
{
	__ASM volatile("mov		r0,		%0" ::"r"(NVIC_SYSPRI2));
	__ASM volatile("mov		r1,		%0" ::"r"(NVIC_PENDSV_PRI));
	__ASM volatile("str		r1,		[r0]\n");
}

//主动触发SV中断
void sche_trigger_pend_sv(void)
{
	__ASM volatile("mov		r0,		%0" ::"r"(NVIC_INT_CTRL));
	__ASM volatile("mov		r1,		%0" ::"r"(NVIC_PENDSV_SET));
	__ASM volatile("str		r1,		[r0]");
}

//获取当前运行的pcb
pcb_s *sche_curr_pcb(void)
{
	return p_current;
}

//关中断（进入原子操作）
void sche_int_enter(void)
{
	__ASM volatile("cpsid       I");
}

//开中断（离开原子操作）
void sche_int_leave(void)
{
	__ASM volatile("cpsie       I");
}

//系统心跳
void sche_tick(void)
{
	//关中断
	sche_int_enter();

	//唤醒休眠完成的进程
	wakeup();

	//获取最高优先级的进程
	p_next = pcb_get_highest_pcb();
	//如果最高优先级进程就是当前进程，则进行跳转返回
	if (p_current == p_next)
	{
		goto _out;
	}
	//触发调度
	sche_trigger_pend_sv();

_out:
	//开中断
	sche_int_leave();
}

//调度任务切换
void sche_switch(void)
{
	//获取最高优先级的进程
	p_next = pcb_get_highest_pcb();
	//如果最高优先级进程就是当前进程，则进行跳转返回
	if (p_current == p_next)
	{
		return;
	}
	//触发调度
	sche_trigger_pend_sv();
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
	sche_int_enter();

	//取得当前运行的进程
	pcb_s *p_curr = sche_curr_pcb();
	//设置休眠tick数
	p_curr->sleep_tick = tick;
	//挂起当前进程
	pcb_block(p_curr);
	//将当前进程
	slist_append(&sleep_list, p_curr, p_curr);

	//触发调度
	sche_trigger_pend_sv();

	//开中断
	sche_int_leave();
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
		pcb_s *pcb = (pcb_s *)(*p)->value;
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
		//下一个节点
		p = &(*p)->next;
	}
}

//STM32的PendSV中断处理函数
void PendSV_Handler(void)
{
	//关中断
	__ASM volatile("cpsid       I");

	//如果是第一次调度
	if (p_current == NULL)
	{
		//将当前进程设置为最高优先级进程
		p_current = pcb_get_highest_pcb();
		goto _no_save;
	}

	//------------------------------------------------------------------------
	//- 保护现场开始
	//------------------------------------------------------------------------
	//新的栈顶位置
	uint32_t new_top = 0;
	//将psp的值取出放入r0
	__ASM volatile("mrs     r0,     psp");
	//减去r4-r11这8个寄存器的地址8 * 4 = 32 = 0x20
	__ASM volatile("subs    r0,     r0, $0x20");
	//将r4-r11保存到栈中
	__ASM volatile("stm     r0,     {r4-r11}");
	//获取新的栈顶位置
	__ASM volatile("mov     %0,     r0"
				   : "=r"(new_top));
	//更新pcb中的栈顶位置
	p_current->p_stack = (void *)new_top;
	//将当前进程修改为下一个需要运行的进程
	p_current = p_next;
	//------------------------------------------------------------------------
	//- 保护现场结束
	//------------------------------------------------------------------------

_no_save:

	//------------------------------------------------------------------------
	//- 还原现场开始
	//------------------------------------------------------------------------
	//将当前进程栈的位置存入r0
	__ASM volatile("mov     r0,     %0"
				   :
				   : "r"(p_current->p_stack));
	//恢复r4-r11这8个寄存器
	__ASM volatile("ldm     r0,     {r4-r11}");
	//加上r4-r11这8个寄存器的地址8 * 4 = 32 = 0x20
	__ASM volatile("adds    r0,     r0, $0x20");
	//将栈顶位置存入psp寄存器中
	__ASM volatile("msr     psp,    r0");
	//------------------------------------------------------------------------
	//- 还原现场结束
	//------------------------------------------------------------------------

	//设置lr寄存器，保证中断返回时使用进程栈(bit2 == 1)
	__ASM volatile("orr         lr,     lr, #0x04");

	//开中断
	__ASM volatile("cpsie       I");
}