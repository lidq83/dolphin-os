#ifndef __DRV_USART1_H
#define __DRV_USART1_H

#include <board.h>

void RCC_Configuration(void);

void GPIO_Configuration(void);

void USART1_Configuration(void);

void USART1_puts(char* s);

#endif