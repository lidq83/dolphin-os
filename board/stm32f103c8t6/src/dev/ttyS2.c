/*
 * task.c
 *
 *  Created on: December 23, 2019
 *      Author: lidq
 */

#include <ttyS2.h>

extern uint8_t *u2_t_buff;
extern buff_s *u2_rx_buff;

int ttyS2_open(struct file *fs);
int ttyS2_close(struct file *fs);
size_t ttyS2_read(struct file *fs, void *buff, size_t size);
size_t ttyS2_write(struct file *fs, const void *buff, size_t size);
int ttyS2_ioctl(struct file *fs, unsigned int cmd, unsigned long arg);

int ttyS2_open(struct file *fs)
{
	return 0;
}

int ttyS2_close(struct file *fs)
{
	return 0;
}

size_t ttyS2_read(struct file *fs, void *buff, size_t size)
{
	char *buff_read = (char *)buff;
	int size_u2 = buff_size(u2_rx_buff);
	if (size_u2 < size)
	{
		size = size_u2;
	}
	for (int i = 0; size > 0 && i < size; i++)
	{
		buff_read[i] = u2_rx_buff->buff[u2_rx_buff->foot];
		u2_rx_buff->foot++;
		u2_rx_buff->foot %= BUFF_SIZE_UART;
	}
	return size;
}

size_t ttyS2_write(struct file *fs, const void *buff, size_t size)
{
	if (size > BUFF_SIZE_UART)
	{
		size = BUFF_SIZE_UART;
	}
	char *p = (char *)buff;
	uart2_send(p, size);
	return size;
}

int ttyS2_ioctl(struct file *fs, unsigned int cmd, unsigned long arg)
{
	return 0;
}

void ttyS2_init(void)
{
	file_operations_s ops = {0};

	ops.open = ttyS2_open;
	ops.close = ttyS2_close;
	ops.write = ttyS2_write;
	ops.read = ttyS2_read;
	ops.ioctl = ttyS2_ioctl;

	fs_register_dev("/dev/ttyS2", ops);
}
