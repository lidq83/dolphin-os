#include <stdint.h>
#include <fcntl.h>
#include <fs.h>
#include <std.h>
// #include <serial1.h>

void std_putchar(char ch)
{
	// serial1_write(ch);
}

int stdout_write(struct file *f, const void *buf, size_t size)
{
	char *p = (char *)buf;
	for (size_t i = 0; i < size; i++)
	{
		std_putchar(p[i]);
	}
	return size;
}

int std_init(void)
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
