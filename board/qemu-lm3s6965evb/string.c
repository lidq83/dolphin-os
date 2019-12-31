/*
 * string.c
 *
 *  Created on: December 25, 2019
 *      Author: lidq
 */
#include <string.h>

void memset(void *mem, int val, unsigned long size)
{
	char *p = (char *)mem;
	char v = (char)val;
	for (unsigned long i = 0; i < size; i++)
	{
		p[i] = v;
	}
}

int strlen(char *str)
{
	int len = 0;
	while(*str != 0)
	{
		len++;
		str++;
	}
	return len;
}