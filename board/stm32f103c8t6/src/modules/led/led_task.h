/*
 * task.h
 *
 *  Created on: December 23, 2019
 *      Author: lidq
 */
#ifndef __SRC_LED_TASK_H
#define __SRC_LED_TASK_H

#include <typedef.h>
#include <sche.h>
#include <sem.h>
#include <led.h>
#include <mm.h>
#include <pcb.h>
#include <uart1.h>

void led_task(void);

#endif
