/*
 * uart.h
 *
 *  Created on: Jun 25, 2017
 *      Author: lidq
 */

#ifndef __SRC_MODULES_UART2_H_
#define __SRC_MODULES_UART2_H_

#include <typedef.h>

int buff_size(void);

u8 Uart2_PutChar(u8 ch);

void Uart2_PutString(u8* buf, u8 len);

void uart2_init();

#endif /* SRC_UART_H_ */
