/*
 * sche.h
 *
 *  Created on: December 23, 2019
 *      Author: lidq
 */
#ifndef __SRC_KERNEL_SCHE_H
#define __SRC_KERNEL_SCHE_H

#include <typedef.h>
#include <pcb.h>

//
void sche_init(void);

//系统心跳
void sche_tick(void);

//调度任务切换
void sche_switch(void);

//主动按tick数休眠
void sleep_ticks(uint32_t tick);

//设置SV中断优先级
void sche_pend_sv_proi(void);

//触发SV中断
void sche_trigger(void);

//获取当前运行的pcb
pcb_s *sche_curr_pcb(void);

//关中断（进入原子操作）
void sche_int_enter(void);

//开中断（离开原子操作）
void sche_int_leave(void);

#endif
