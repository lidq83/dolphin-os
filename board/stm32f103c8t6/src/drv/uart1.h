/*
 * uart.h
 *
 *  Created on: Jun 25, 2017
 *      Author: lidq
 */

#ifndef __SRC_MODULES_UART1_H_
#define __SRC_MODULES_UART1_H_

#include <typedef.h>

void UART1_GPIO_Configuration(void);

void UART1_Configuration(void);

uint8_t Uart1_PutChar(uint8_t ch);

void Uart1_PutString(uint8_t* buf, uint8_t len);

void USART1_IRQHandler(void);

void uart1_init();

#endif /* SRC_UART_H_ */
