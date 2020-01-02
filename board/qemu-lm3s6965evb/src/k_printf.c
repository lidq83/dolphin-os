#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <k_printf.h>

volatile char *const UART0DR = (char *)0x4000C000;
volatile char _buff[256] = {0};
volatile char _temp[256] = {0};
volatile char _num[20] = "0123456789ABCDEFG";

void putchar(char ch)
{
    *UART0DR = ch;
    return ch;
}

/*
 * number_to_str : 将整数转为字符串
 *  - int tty_id : tty编号
 *  - char *buff : 数据地址
 *  - int number : 整数
 *  - int hex : 10进制或16进制
 * return : void
 */
void number_to_str(char *buff, int number, int hex)
{
    int i = 0;
    int length = 0;
    int rem;
    char sign = '+';

    //反向加入temp
    _temp[i++] = '\0';
    if (number < 0)
    {
        sign = '-';
        number = 0 - number;
    }
    else if (number == 0)
    {
        _temp[i++] = '0';
    }

    //将数字转为字符串
    while (number > 0)
    {
        rem = number % hex;
        _temp[i++] = _num[rem];
        number = number / hex;
    }
    //处理符号
    if (sign == '-')
    {
        _temp[i++] = sign;
    }
    length = i;

    //返向拷贝到buff缓冲区
    for (i = length - 1; i >= 0; i--)
    {
        *buff++ = _temp[i];
    }
}

/*
 * k_puts : 显示字符串
 *  - int tty_id : tty编号
 *  - char *str : 字符串
 * return : void
 */
int k_puts(char *str)
{
    int count = 0;
    while (*str != '\0')
    {
        putchar(*str++);
        count++;
    }
    return count;
}

/***
 * k_printf标准输出函数
 * char *fmt: 格式化字符串
 * 动态参数
 * return: 显示字符个数
 */
int k_printf(char *fmt, ...)
{
    //显示字符串指针
    char *str;
    //显示字符变量
    char ch;
    //显示字符总数
    int count = 0;

    //动态参数
    va_list args;
    //初始化动态参数
    va_start(args, fmt);

    //读到\0为结束
    while (*fmt != 0)
    {
        //格式化标记%
        if (*fmt == '%')
        {
            //显示一个字符
            if ('c' == *(fmt + 1))
            {
                ch = va_arg(args, char);
                putchar(ch);
                count++;
                fmt += 2;
            }
            //显示字符串
            else if ('s' == *(fmt + 1))
            {
                str = va_arg(args, char *);
                count += k_puts(str);
                fmt += 2;
            }
            //显示整数
            else if ('d' == *(fmt + 1))
            {
                number_to_str(_buff, va_arg(args, int), 10);
                count += k_puts(_buff);
                fmt += 2;
            }
            //显示无符号16进制整数
            else if ('x' == *(fmt + 1))
            {
                uint32_t num = va_arg(args, uint32_t);
                uint32_t nl = num & 0xffff;
                uint32_t nh = (num >> 16) & 0xffff;
                count += k_puts("0x");
                if (nh == 0)
                {
                    number_to_str(_buff, nl, 16);
                    count += k_puts(_buff);
                }
                else
                {
                    number_to_str(_buff, nh, 16);
                    count += k_puts(_buff);

                    number_to_str(_buff, nl, 16);
                    int zero = 4 - strlen(_buff);
                    for (int i = 0; i < zero; i++)
                    {
                        putchar('0');
                    }
                    count += k_puts(_buff);
                }
                fmt += 2;
            }
        }
        //显示普通字符
        else
        {
            putchar(*fmt++);
            count++;
        }
    }

    va_end(args);

    return count;
}