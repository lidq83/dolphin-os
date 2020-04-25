/*
 * uart.h
 *
 *  Created on: Jun 25, 2017
 *      Author: lidq
 */

#ifndef __SRC_MODULES_UART2_H_
#define __SRC_MODULES_UART2_H_

#include <typedef.h>
#include <buff_s.h>

#define BUATRATE_UART2 (115200)

void uart2_init();

void uart2_send(void *buff, size_t size);

#endif /* SRC_UART_H_ */
