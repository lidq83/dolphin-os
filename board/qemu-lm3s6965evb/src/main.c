#include <core.h>
#include <pcb.h>
#include <sche.h>
#include <fcntl.h>
#include <fs.h>
#include "debug.h"
#include "systick.h"

#define DEBUG_LOG printf_str

#define STATCK_SIZE 128
#define STATCK_SIZE_LONG 1024

//任务0入口函数
void task0_entry(void *arg)
{
	while (1)
	{
		sleep_ticks(1000);
		DEBUG_LOG("run thread0\n");
	}
}

//任务1入口函数
void task1_entry(void *arg)
{
	while (1)
	{
		sleep_ticks(1000);
		DEBUG_LOG("run thread1\n");
	}
}

int t_open(struct file *f)
{
	DEBUG_LOG("open\n");
	return 0;
}

int t_close(struct file *f)
{
	DEBUG_LOG("close\n");
	return 0;
}

int t_read(struct file *f, void *buf, size_t size)
{
	DEBUG_LOG("t_read\n");
	return 10;
}

int t_write(struct file *f, const void *buf, size_t size)
{
	DEBUG_LOG("t_write\n");
	return 20;
}

int t_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
	DEBUG_LOG("ioctl\n");
	return 0;
}

//任务2入口函数
void task2_entry(void *arg)
{
	file_operations_s ops = {0};
	ops.open = t_open;
	ops.close = t_close;
	ops.read = t_read;
	ops.write = t_write;
	ops.ioctl = t_ioctl;

	DEBUG_LOG("task2\n");

	int ret = fs_register_dev("/dev/task2", ops);
	if (ret < 0)
	{
		DEBUG_LOG("reg err!\n");
		return;
	}

	int fd = open("/dev/task2", 0, 0);
	printf("open fd %d\n", fd);
	if (fd < 0)
	{
		DEBUG_LOG("open err!\n");
		return;
	}

	char buff[100] = {0};
	for (int i = 0;; i++)
	{
		int len = read(fd, buff, 100);
		printf("read %d\n", len);
		sleep_ticks(500);

		len = write(fd, buff, 100);
		printf("write %d\n", len);
		sleep_ticks(500);
	}

	ioctl(fd, 1, 2);
	close(fd);
}

int main(int argc, char **argv)
{
	DEBUG_LOG("Dolphin-OS startup.\n");

	//操作系统初始化，为TCB分配内存空间，同时启动空闲任务
	kernel_startup();

	//创建用户任务
	uint8_t *task0_stack = malloc(STATCK_SIZE);
	uint8_t *task1_stack = malloc(STATCK_SIZE);
	uint8_t *task2_stack = malloc(STATCK_SIZE_LONG);

	//系统时钟初始化，在系统中断服务程序中任务调度
	pcb_create(0, task0_entry, (void *)0, &task0_stack[STATCK_SIZE]);
	pcb_create(1, task1_entry, (void *)0, &task1_stack[STATCK_SIZE]);
	pcb_create(2, task2_entry, (void *)0, &task2_stack[STATCK_SIZE_LONG]);

	systick_init();

	while (1)
	{
	}
}
