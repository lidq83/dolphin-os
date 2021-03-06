#include <typedef.h>
#include <stdint.h>
#include <fcntl.h>
#include <fs.h>
#include <stddev.h>

void stddev_putchar(char ch)
{
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
