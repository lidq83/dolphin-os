#include <stdint.h>
#include <fcntl.h>
#include <fs.h>
#include <stddev.h>
#include <uart1.h>
#include <virtual_uart.h>
#include <buff_s.h>

#define STD_OUT virtual_uart_send_char

int stdout_write(struct file *f, const void *buf, size_t size)
{
	char *p = (char *)buf;
	for (size_t i = 0; i < size; i++)
	{
		STD_OUT(p[i]);
	}
	return size;
}

int stdin_init(void)
{
	return 0;
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

int stddev_init(void)
{
	stdin_init();
	stdout_init();
	stderr_init();

	return 0;
}
