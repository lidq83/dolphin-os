/*
 * kernel.c
 *
 *  Created on: December 20, 2019
 *      Author: lidq
 */

#include <core.h>
#include <pcb.h>
#include <sche.h>

static uint8_t idle_stack[128] = {0};

static void process_idle(void);

void process_idle(void)
{
    static uint32_t idle_ind = 0;
    
    while (1)
    {
        idle_ind++;
    }
}

void kernel_startup(void)
{
    pcb_s *idle_pcb = pcb_create(31, &process_idle, NULL, &idle_stack[128]);
    pcb_ready(idle_pcb);

    sche_pend_sv_proi();
}
