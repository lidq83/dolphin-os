/*
 * k_printf.c
 *
 *  Created on: January 2, 2020
 *      Author: lidq
 */

#include <k_printf.h>
#include <fcntl.h>
#include <stdarg.h>
#include <mm.h>
#include <sem.h>
#include <buff_s.h>

static char *buff = NULL;
static sem_t sem_p = {0};

int k_printf(char *fmt, ...)
{
	if (buff == NULL)
	{
		buff = malloc(BUFF_SIZE_UART);
		sem_init(&sem_p, 1);
	}

	if (buff == NULL)
	{
		return -1;
	}

	sem_wait(&sem_p);

	va_list ap;
	int ret;
	va_start(ap, fmt);
	ret = vsnprintf(buff, BUFF_SIZE_UART, fmt, ap);
	va_end(ap);
	
	write(1, buff, ret);

	sem_post(&sem_p);
	return ret;
}