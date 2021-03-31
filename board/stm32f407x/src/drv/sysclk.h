/*
 * sysclk.h
 *
 *  Created on: Dec 24, 2019
 *      Author: lidq
 */

#ifndef __MODULES_SYSCLK_H
#define __MODULES_SYSCLK_H

#include <board.h>

#include "stm32f4xx.h"

#define TIME_TICK_MS 1
#define TIME_TICK_US (TIME_TICK_MS * 1000)

#define TIME_SYS_ST_DEBUG 0

#define TIME_CMD_ENABLE(nTIM) nTIM->CR1 |= (uint16_t)TIM_CR1_CEN;
#define TIME_CMD_DISABLE(nTIM) nTIM->CR1 &= (uint16_t)~TIM_CR1_CEN;

void sysclk_init(void);

void time2_close_interrput(void);

#endif
