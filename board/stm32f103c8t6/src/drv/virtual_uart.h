#ifndef __VIRTUAL_UART_H
#define __VIRTUAL_UART_H

#include <typedef.h>

void virtual_uart_init();

void virtual_uart_send_char(char ch);

void virtual_uart_send_buff(char *buff, int len);

#endif
