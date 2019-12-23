/*
 * sche.h
 *
 *  Created on: December 20, 2019
 *      Author: lidq
 */
#ifndef __SRC_KERNEL_SCHE_H
#define __SRC_KERNEL_SCHE_H

#include <typedef.h>
#include <pcb.h>

void sche_tick(void);

void sleep_ticks(uint32_t tick);

void sche_pend_sv_proi(void);

void sche_trigger_pend_sv(void);

pcb_s *sche_curr_pcb(void);

void sche_int_enter(void);

void sche_int_leave(void);

#endif
