/*
 * k_scanf.h
 * 
 *  Created on: January 3, 2020
 *      Author: lidq
 */
#ifndef _SRC_LIB_K_SCANF_H
#define _SRC_LIB_K_SCANF_H

#ifndef NULL
#define NULL (void *)(0)
#endif

char k_getchar(void);

int k_gets(char *str);

int k_scanf(char *fmt, ...);

#endif
