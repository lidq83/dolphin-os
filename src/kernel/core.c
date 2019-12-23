/*
 * kernel.c
 *
 *  Created on: December 20, 2019
 *      Author: lidq
 */

#include <core.h>
#include <pcb.h>
#include <sche.h>

//空闲进程栈
static uint8_t idle_stack[128] = {0};

//空闲进程
static void process_idle(void);

//空闲进程
void process_idle(void)
{
    static uint32_t idle_ind = 0;
    
    while (1)
    {
        idle_ind++;
    }
}

//启动内核程序
void kernel_startup(void)
{
    //创建空闲进程，优先级为最低
    pcb_s *idle_pcb = pcb_create(31, &process_idle, NULL, &idle_stack[128]);
    //将空闲进程放入就绪队列
    pcb_ready(idle_pcb);
    //设置SV中断优先级
    sche_pend_sv_proi();
}
