#include <stdint.h>
#include <fcntl.h>
#include <fs.h>
#include <sche.h>
#include <stddev.h>

#define BUFF_SIZE (16)

typedef struct buff_s
{
    char buff[BUFF_SIZE];
    int head;
    int foot;
} buff_s;

static sem_t sem_stdin_rw = {0};
static sem_t sem_stdin_cnt = {0};
static buff_s stdin_buff = {0};

#define UART_DR(baseaddr) (*(unsigned int *)(baseaddr))
#define UART_FR(baseaddr) (*(((unsigned int *)(baseaddr)) + 6))

enum
{
    UART_FR_RXFE = 0x10,
    UART_FR_TXFF = 0x20,
    UART0_ADDR = 0x4000C000
};

int buff_append(char ch)
{
    stdin_buff.buff[stdin_buff.head] = ch;
    stdin_buff.head++;
    stdin_buff.head %= BUFF_SIZE;
    if (stdin_buff.head == stdin_buff.foot)
    {
        stdin_buff.foot++;
        stdin_buff.foot %= BUFF_SIZE;
    }
    return 0;
}

int buff_size(void)
{
    int size = stdin_buff.head - stdin_buff.foot;
    if (size < 0)
    {
        size += BUFF_SIZE;
    }
    return size;
}

void stddev_putchar(char ch)
{
    if (ch == 0x8)
    {
        UART_DR(UART0_ADDR) = ch;
        UART_DR(UART0_ADDR) = ' ';
        UART_DR(UART0_ADDR) = ch;
        return;
    }

    if (ch == '\t')
    {
        ch = ' ';
    }

    UART_DR(UART0_ADDR) = ch;

    if (ch == '\n')
    {
        UART_DR(UART0_ADDR) = '\r';
        return;
    }

    if (ch == '\r')
    {
        UART_DR(UART0_ADDR) = '\n';
        return;
    }
}

void stdin_input_check(void)
{
    while (1)
    {
        while (UART_FR(UART0_ADDR) & UART_FR_RXFE)
        {
            sleep_ticks(10);
        }
        char ch = UART_DR(UART0_ADDR);
        stddev_putchar(ch);

        sem_wait(&sem_stdin_rw);
        buff_append(ch);
        int b_size = buff_size();
        if (b_size < 16)
        {
            sem_post(&sem_stdin_cnt);
        }
        sem_post(&sem_stdin_rw);
    }
}

int stdin_read(struct file *f, void *buf, size_t size)
{
    char *p = (char *)buf;
    int r_size = 0;
    for (size_t i = 0; i < size; i++)
    {
        sem_wait(&sem_stdin_cnt);
        sem_wait(&sem_stdin_rw);
        int b_size = buff_size();
        if (b_size > 0)
        {
            p[i] = stdin_buff.buff[stdin_buff.foot];
            stdin_buff.foot++;
            stdin_buff.foot %= BUFF_SIZE;
            r_size++;
        }
        sem_post(&sem_stdin_rw);
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
    stdout_init();
    stderr_init();
    stdin_init();

    return 0;
}

int stdin_init(void)
{
    sem_init(&sem_stdin_rw, 1);
    sem_init(&sem_stdin_cnt, 0);
    pcb_create(29, stdin_input_check, NULL, 512);
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
