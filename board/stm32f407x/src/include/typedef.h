#ifndef __TYPEDEF_H
#define __TYPEDEF_H

#include <stm32f4xx_conf.h>
#include <stdio.h>
#include <math.h>
#include <misc.h>
#include <stdint.h>

#define PRIO_TASK_LED (20)
#define PRIO_TASK_SEM_POST (11)
#define PRIO_TASK_SEM_WAIT (12)
#define PRIO_TASK_STDIO (18)
#define PRIO_TASK_USB (8)
#define PRIO_TASK_UARTDMA (6)

#define BUFF_SIZE (16)
#define USB_BUFF_SIZE (1024)

typedef struct buff_s
{
    char buff[BUFF_SIZE];
    int head;
    int foot;
} buff_s;

typedef struct usb_buff_s
{
    char buff[USB_BUFF_SIZE];
    int head;
    int foot;
} usb_buff_s;

#endif
