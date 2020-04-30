/*
 * task.c
 *
 *  Created on: December 23, 2019
 *      Author: lidq
 */

#include <ttyS2.h>

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
	uint8_t *p = (uint8_t *)buff;
	size_t read_len = 0;
	for (int i = 0; i < size; i++)
	{
		if (!serial2_read(&p[read_len]))
		{
			return read_len;
		}
		read_len++;
	}
	return size;
}

size_t ttyS2_write(struct file *fs, const void *buff, size_t size)
{
	uint8_t *p = (uint8_t *)buff;
	for (int i = 0; i < size; i++)
	{
		serial2_write(p[i]);
	}
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
