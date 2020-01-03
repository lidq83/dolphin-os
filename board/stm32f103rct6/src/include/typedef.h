#ifndef __TYPEDEF_H
#define __TYPEDEF_H

#include <stm32f10x.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_tim.h>
#include <stm32f10x_usart.h>
#include <stm32f10x_exti.h>
#include <stm32f10x_i2c.h>
#include <system_stm32f10x.h>
#include <stdio.h>
#include <math.h>
#include <misc.h>
#include <stdint.h>

#define PRIO_TASK_0 (10)
#define PRIO_TASK_1 (12)
#define PRIO_TASK_2 (14)
#define PRIO_TASK_3 (16)
#define PRIO_TASK_4 (18)

#define BUFF_SIZE (16)

typedef struct buff_s
{
    char buff[BUFF_SIZE];
    int head;
    int foot;
} buff_s;

#endif
