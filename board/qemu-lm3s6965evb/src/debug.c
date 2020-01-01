
#include <stdint.h>
#include <stdarg.h>
#include <string.h>

volatile uint32_t *const UART0DR = (uint32_t *)0x4000C000;

char send_char(uint8_t *ch)
{
    *UART0DR = *ch;
    return *ch;
}

int printf_char(char ch)
{
    uint8_t c1 = (uint8_t)'\r';
    uint8_t c2 = 0;
    c2 = (uint8_t)ch;
    send_char(&c2);

    if (((uint8_t)'\n') == c2)
    {
        send_char(&c1);
    }

    return 0;
}

int printf_str(char *str)
{
    while (str && (*str != (char)'\0'))
    {
        printf_char(*str);
        str++;
    }

    return 0;
}

void putchar(char ch)
{
    printf_char(ch);
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
    char temp[0x800];
    char num[0x20] = "0123456789ABCDEFG";

    int i = 0;
    int length = 0;
    int rem;
    char sign = '+';

    //反向加入temp
    temp[i++] = '\0';
    if (number < 0)
    {
        sign = '-';
        number = 0 - number;
    }
    else if (number == 0)
    {
        temp[i++] = '0';
    }

    //将数字转为字符串
    while (number > 0)
    {
        rem = number % hex;
        temp[i++] = num[rem];
        number = number / hex;
    }
    //处理符号
    if (sign == '-')
    {
        temp[i++] = sign;
    }
    length = i;

    //返向拷贝到buff缓冲区
    for (i = length - 1; i >= 0; i--)
    {
        *buff++ = temp[i];
    }
}

/*
 * puts : 显示字符串
 *  - int tty_id : tty编号
 *  - char *str : 字符串
 * return : void
 */
int puts(char *str)
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
 * 标准输出函数
 * char *fmt: 格式化字符串
 * 动态参数
 * return: 显示字符个数
 */
int printf(char *fmt, ...)
{
    //显示数字缓冲区
    char buff[0x800];
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
    while (*fmt != '\0')
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
                count += puts(str);
                fmt += 2;
            }
            //显示整数
            else if ('d' == *(fmt + 1))
            {
                number_to_str(buff, va_arg(args, int), 10);
                count += puts(buff);
                fmt += 2;
            }
            //显示无符号16进制整数
            else if ('x' == *(fmt + 1))
            {
                uint32_t num = va_arg(args, uint32_t);
                uint32_t nl = num & 0xffff;
                uint32_t nh = (num >> 16) & 0xffff;
                count += puts("0x");
                if (nh == 0)
                {
                    number_to_str(buff, nl, 16);
                    count += puts(buff);
                }
                else
                {
                    number_to_str(buff, nh, 16);
                    count += puts(buff);

                    number_to_str(buff, nl, 16);
                    int zero = 4 - strlen(buff);
                    for (int i = 0; i < zero; i++)
                    {
                        putchar('0');
                    }
                    count += puts(buff);
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