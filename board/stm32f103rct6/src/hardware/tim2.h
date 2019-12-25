/*
 * pwm.h
 *
 *  Created on: Dec 24, 2019
 *      Author: lidq
 */

#include <typedef.h>

#ifndef __SRC_MODULES_PWM_H
#define __SRC_MODULES_PWM_H

void tim2_pwm_init(void);

void tim2_pwm_set(uint16_t *pwm);

#endif