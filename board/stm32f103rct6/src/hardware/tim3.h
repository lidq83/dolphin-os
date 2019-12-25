#ifndef __SRC_MODULES_TIM3_H
#define __SRC_MODULES_TIM3_H

#include <typedef.h>

void tim3_in_init(void);

void tim3_irq_init(void);

void tim3_gpio_init(void);

void tim3_capture_init(void);

void tim3_set_st(uint8_t status);

uint16_t tim3_get_value();

#endif
