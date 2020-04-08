/*
 * k_printf.h
 *
 *  Created on: January 2, 2020
 *      Author: lidq
 */
#ifndef _SRC_LIB_K_PRINTF_H
#define _SRC_LIB_K_PRINTF_H

#ifndef NULL
#define NULL (void *)(0)
#endif

void number_to_str(char *buff, int number, int hex);

int k_puts(char *str);

int k_printf(char *fmt, ...);

#endif
