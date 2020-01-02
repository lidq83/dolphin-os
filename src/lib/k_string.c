/*
 * k_string.c
 *
 *  Created on: December 25, 2019
 *      Author: lidq
 */
#include <k_string.h>

void k_memset(void *mem, int val, unsigned long size)
{
	char *p = (char *)mem;
	char v = (char)val;
	for (unsigned long i = 0; i < size; i++)
	{
		p[i] = v;
	}
}

void k_memcpy(void *tar, void *src, unsigned long size)
{
	char *p = (char *)tar;
	char *q = (char *)src;
	for (unsigned long i = 0; i < size; i++)
	{
		*p++ = *q++;
	}
}

int k_strcpy(char *tar, char *src)
{
	int i = 0;
	while (*src != 0)
	{
		*tar++ = *src++;
		i++;
	}
	return i;
}

int k_strncpy(char *tar, char *src, int len)
{
	int i = 0;
	for (i = 0; i < len && *src != 0; i++)
	{
		*tar++ = *src++;
	}
	return i;
}

int k_strcmp(char *str1, char *str2)
{
	int ret = 0;
	while (*str1 != 0 && *str2 != 0)
	{
		if (*str1 != *str2)
		{
			ret = *str1 - *str2;
			goto _out;
		}
		str1++;
		str2++;
	}
	if (*str1 == *str2)
	{
		return 0;
	}

	ret = *str1 - *str2;

_out:
	if (ret < 0)
	{
		return -1;
	}
	if (ret > 0)
	{
		return 1;
	}
	return 0;
}

int k_strlen(char *str)
{
	int len = 0;
	while (*str != 0)
	{
		len++;
		str++;
	}
	return len;
}