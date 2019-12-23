/*
 * pcb.h
 *
 *  Created on: December 20, 2019
 *      Author: lidq
 */
#ifndef __SRC_KERNEL_PCB_H
#define __SRC_KERNEL_PCB_H

#include <stdint.h>

#define PROCESS_CNT (32)

typedef struct pcb_s
{
    //进程栈顶地址
    void *p_stack;
    //优先级由高0到低32
    uint8_t prio;
    //任务休眠ticks
    uint32_t sleep_tick;
    //任务状态
    uint8_t status;
} pcb_s;

pcb_s *pcb_create(uint8_t prio, void *p_entry, void *p_arg, uint32_t *stack);

void pcb_ready(pcb_s *pcb);

void pcb_block(pcb_s *pcb);

pcb_s* pcb_get_highest_pcb(void);

#endif