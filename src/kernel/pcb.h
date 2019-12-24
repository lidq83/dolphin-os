/*
 * pcb.h
 *
 *  Created on: December 20, 2019
 *      Author: lidq
 */
#ifndef __SRC_KERNEL_PCB_H
#define __SRC_KERNEL_PCB_H

#include <stdint.h>

//支持最多32个进程
#define PROCESS_CNT (32)

//进程控制块Process Control Block
typedef struct pcb_s
{
    //进程栈顶地址
    void *p_stack;
    //优先级由高0到低32
    uint8_t prio;
    //任务休眠ticks
    uint32_t sleep_tick;
} pcb_s;

void *stack_init(void *p_entry, void *p_arg, void *stack);

//创建一个进程
pcb_s *pcb_create(uint8_t prio, void *p_entry, void *p_arg, uint32_t *stack);

//将进程加入就绪队列
void pcb_ready(pcb_s *pcb);

//将进程由就绪队列挂起
void pcb_block(pcb_s *pcb);

//获取优先级最高的进程索引
uint32_t pcb_get_highest_prio(void);

//获取就绪队列中优先级最高的pcb
pcb_s* pcb_get_highest_pcb(void);

#endif
