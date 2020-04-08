#include <stdint.h>
#include <fcntl.h>
#include <fs.h>
#include <stddev.h>
#include <uart1.h>
#include <virtual_uart.h>
#include <buff_s.h>

// #define STD_OUT		Uart1_PutChar
#define STD_OUT		virtual_uart_send_char

extern buff_s rx_buff;

sem_t sem_stdin_cnt = {0};

//UART1
void stddev_putchar(char ch)
{
    STD_OUT(ch);
    // if (ch == '\n')
    // {
    //     STD_OUT('\r');
    // }
    // if (ch == '\r')
    // {
    //     STD_OUT('\n');
    // }
}

void stdin_post_sem(void)
{
    sem_post(&sem_stdin_cnt);
}

int stdin_read(struct file *f, void *buf, size_t size)
{
    char *p = (char *)buf;
    int r_size = 0;
    for (size_t i = 0; i < size; i++)
    {
        sem_wait(&sem_stdin_cnt);
        int b_size = buff_size(&rx_buff);
        if (b_size > 0)
        {
            p[i] = rx_buff.buff[rx_buff.foot];
            rx_buff.foot++;
            rx_buff.foot %= BUFF_SIZE_UART;
            r_size++;
        }
    }
    return r_size;
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
    sem_init(&sem_stdin_cnt, 0);
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
