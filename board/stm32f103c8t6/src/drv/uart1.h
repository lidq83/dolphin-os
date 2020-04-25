/*
 * uart.h
 *
 *  Created on: Jun 25, 2017
 *      Author: lidq
 */

#ifndef __SRC_MODULES_UART1_H_
#define __SRC_MODULES_UART1_H_

#include <typedef.h>
#include <buff_s.h>

#define BUATRATE_UART1 (115200)

void uart1_init();

void uart1_send(void *buff, size_t size);

#endif /* SRC_UART_H_ */
