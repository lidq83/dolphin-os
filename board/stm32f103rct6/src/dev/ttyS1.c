/*
 * task.c
 *
 *  Created on: December 23, 2019
 *      Author: lidq
 */

#include <ttyS1.h>

extern uint8_t *u1_t_buff;
extern buff_s *u1_rx_buff;

int ttyS1_open(struct file *fs);
int ttyS1_close(struct file *fs);
size_t ttyS1_read(struct file *fs, void *buff, size_t size);
size_t ttyS1_write(struct file *fs, const void *buff, size_t size);
int ttyS1_ioctl(struct file *fs, unsigned int cmd, unsigned long arg);

int ttyS1_open(struct file *fs)
{
	return 0;
}

int ttyS1_close(struct file *fs)
{
	return 0;
}

size_t ttyS1_read(struct file *fs, void *buff, size_t size)
{
	char *buff_read = (char *)buff;
	int size_u1 = buff_size(u1_rx_buff);
	if (size_u1 < size)
	{
		size = size_u1;
	}
	for (int i = 0; size > 0 && i < size; i++)
	{
		buff_read[i] = u1_rx_buff->buff[u1_rx_buff->foot];
		u1_rx_buff->foot++;
		u1_rx_buff->foot %= BUFF_SIZE_UART;
	}
	return size;
}

size_t ttyS1_write(struct file *fs, const void *buff, size_t size)
{
	if (size > BUFF_SIZE_UART)
	{
		size = BUFF_SIZE_UART;
	}
	char *p = (char *)buff;
	uart1_send(p, size);
	return size;
}

int ttyS1_ioctl(struct file *fs, unsigned int cmd, unsigned long arg)
{
	return 0;
}

void ttyS1_init(void)
{
	file_operations_s ops = {0};

	ops.open = ttyS1_open;
	ops.close = ttyS1_close;
	ops.write = ttyS1_write;
	ops.read = ttyS1_read;
	ops.ioctl = ttyS1_ioctl;

	fs_register_dev("/dev/ttyS1", ops);
}
