/*
 * k_scanf.c
 *
 *  Created on: January 3, 2020
 *      Author: lidq
 */

#include <stdint.h>
#include <stdarg.h>
#include <k_string.h>
#include <k_printf.h>
#include <k_scanf.h>
#include <fcntl.h>

char k_getchar(void)
{
    char ch = 0;
    read(0, &ch, sizeof(char));
    return ch;
}

int k_gets(char *str)
{
    char ch, *p = str;
    while ((ch = k_getchar()) != '\n')
    {
        if (ch == '\r')
        {
            break;
        }

        if (ch == 0x8 && p > str)
        {
            p--;
        }
        else
        {
            *p = ch;
            p++;
        }
    }
    *p = '\0';
}

char k_get_char(void)
{
    char ch = 0;
    char ch_n = 0;
    do
    {
        ch_n = k_getchar();
        if (ch_n == '\n' || ch_n == '\r' || ch_n == '\t' || ch_n == ',' || ch_n == ' ')
        {
            break;
        }
        ch = ch_n;
    } while (1);
    return ch;
}

int k_get_int(void)
{
    char str[0x10];
    char ch, *p = str;
    char sign = 0;
    do
    {
        ch = k_getchar();
        if (ch == 0x8)
        {
            if (p > str)
            {
                p--;
            }
        }
        else if (sign == 0 && (ch == '+' || ch == '-'))
        {
            sign = ch;
        }
        else if (ch == '\n' || ch == '\r' || ch == ',' || ch == ' ' || ch == '\t')
        {
            break;
        }
        else // if (ch >= '0' && ch <= '9')
        {
            *p = ch;
            p++;
        }
    } while (1);

    *p = '\0';
    p = str;

    int num = 0;
    for (int i = 0; p[i] != '\0'; i++)
    {
        if (i == 0)
        {
            num = (int)p[i] - 48;
        }
        else
        {
            num *= 10;
            num += (int)p[i] - 48;
        }
    }
    if (sign == '-')
    {
        return -num;
    }
    return num;
}

int k_scanf(char *fmt, ...)
{
    int count = 0;

    va_list args;
    va_start(args, fmt);
    while (*fmt != '\0')
    {
        if (*fmt == '%')
        {
            if ('c' == *(fmt + 1))
            {
                char *p = (char *)va_arg(args, int);
                *p = k_get_char();
                fmt += 2;
            }
            else if ('s' == *(fmt + 1))
            {
                char *str = (char *)va_arg(args, int);
                k_gets(str);
                fmt += 2;
            }
            else if ('d' == *(fmt + 1))
            {
                int *p = (int *)va_arg(args, int);
                *p = k_get_int();
                fmt += 2;
            }
            else
            {
                break;
            }
        }
        else
        {
            fmt++;
        }
    }

    va_end(args);

    return count;
}