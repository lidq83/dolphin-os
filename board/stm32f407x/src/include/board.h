#ifndef __BOARD_H_
#define __BOARD_H_

#include <board.h>
#include <math.h>
#include <misc.h>
#include <stdint.h>
#include <stdio.h>
#include <stm32f4xx_conf.h>

#ifndef MM_SIZE_HEAP
#define MM_SIZE_HEAP (1024 * 112)
#endif

#define MM_REGION2
#define MM_REGION2_ADDR (0x10000000) // ~0x1000FFFF
#define MM_REGION2_SIZE (0x10000) //64KB

#define PRIO_HMC5883L (28)
#define PRIO_LED (29)

#define SERIAL1_BAUTRATE (115200)

#define LED_CNT (2)

#define PRIO_HMC5883L (28)
#define PRIO_LED (29)

#endif