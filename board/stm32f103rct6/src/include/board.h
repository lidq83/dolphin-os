#ifndef __BOARD_H_
#define __BOARD_H_

#include <math.h>
#include <misc.h>
#include <mm.h>
#include <stdint.h>
#include <stdio.h>
#include <stm32f10x.h>
#include <stm32f10x_adc.h>
#include <stm32f10x_bkp.h>
#include <stm32f10x_can.h>
#include <stm32f10x_crc.h>
#include <stm32f10x_dac.h>
#include <stm32f10x_dbgmcu.h>
#include <stm32f10x_dma.h>
#include <stm32f10x_exti.h>
#include <stm32f10x_flash.h>
#include <stm32f10x_fsmc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_i2c.h>
#include <stm32f10x_iwdg.h>
#include <stm32f10x_pwr.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_rtc.h>
#include <stm32f10x_sdio.h>
#include <stm32f10x_spi.h>
#include <stm32f10x_tim.h>
#include <stm32f10x_usart.h>
#include <stm32f10x_wwdg.h>
#include <system_stm32f10x.h>

#ifndef MM_SIZE_HEAP
#define MM_SIZE_HEAP (1024 * 36)
#endif

#ifndef SERIAL_BAUTRATE
#define SERIAL_BAUTRATE (115200)
#endif

#define LED_CNT (2)

#endif