/*
 * task.c
 *
 *  Created on: December 23, 2019
 *      Author: lidq
 */

#include <ttyS1.h>

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
	uint8_t *p = (uint8_t *)buff;
	size_t read_len = 0;
	for (int i = 0; i < size; i++)
	{
		if (!serial1_read(&p[read_len]))
		{
			return read_len;
		}
		read_len++;
	}
	return size;
}

size_t ttyS1_write(struct file *fs, const void *buff, size_t size)
{
	uint8_t *p = (uint8_t *)buff;
	for (int i = 0; i < size; i++)
	{
		serial1_write(p[i]);
	}
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
