#include <stdint.h>
#include <fcntl.h>
#include <fs.h>
#include <stddev.h>
#include <uart1.h>
#include <virtual_uart.h>
#include <buff_s.h>

#define STD_OUT		virtual_uart_send_char

void stddev_putchar(char ch)
{
    STD_OUT(ch);
}

int stdin_read(struct file *f, void *buf, size_t size)
{
    return 0;
}

int stdout_write(struct file *f, const void *buf, size_t size)
{
    char *p = (char *)buf;
    for (size_t i = 0; i < size; i++)
    {
        stddev_putchar(p[i]);
    }
    return size;
}

int stddev_init(void)
{
    stdin_init();
    stdout_init();
    stderr_init();

    return 0;
}

int stdin_init(void)
{
    file_operations_s ops = {0};
    ops.read = stdin_read;
    return fs_register_dev("/dev/stdin", ops);
}

int stdout_init(void)
{
    file_operations_s ops = {0};
    ops.write = stdout_write;
    return fs_register_dev("/dev/stdout", ops);
}

int stderr_init(void)
{
    return 0;
}
