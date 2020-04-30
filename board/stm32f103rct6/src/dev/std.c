#include <stdint.h>
#include <fcntl.h>
#include <fs.h>
#include <std.h>
#include <virtual_uart.h>
#include <buff_s.h>

int stdout_write(struct file *f, const void *buf, size_t size)
{
	virtual_uart_send_buff((char *)buf, size);
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

int std_init(void)
{
	stdin_init();
	stdout_init();
	stderr_init();

	return 0;
}
