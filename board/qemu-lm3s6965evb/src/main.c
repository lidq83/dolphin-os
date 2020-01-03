#include <core.h>
#include <pcb.h>
#include <sche.h>
#include <fcntl.h>
#include <fs.h>
#include <stddev.h>
#include <k_printf.h>
#include <k_scanf.h>
#include "systick.h"

#define STATCK_SIZE 1024
#define STATCK_SIZE_LONG 1024

//任务0入口函数
void task0_entry(void *arg)
{
	uint32_t i = 0;
	while (1)
	{
		sleep_ticks(100);
		k_printf("run process0 i = %d\n", i++);
	}
}

//任务1入口函数
void task1_entry(void *arg)
{
	uint32_t i = 0;
	while (1)
	{
		sleep_ticks(100);
		k_printf("run process1 i = %d\n", i++);
	}
}

void task3_entry(void *arg)
{
	int a = 0;
	int b = 0;
	while (1)
	{
		k_printf("int a = ?, b = ?\n");
		k_scanf("%d,%d", &a, &b);
		k_printf("a + b = %d\n", a + b);
		k_printf("a * b = %d\n", a * b);
	}
}

int t_open(struct file *f)
{
	k_printf("open\n");
	return 0;
}

int t_close(struct file *f)
{
	k_printf("close\n");
	return 0;
}

int t_read(struct file *f, void *buf, size_t size)
{
	k_printf("t_read\n");
	return 10;
}

int t_write(struct file *f, const void *buf, size_t size)
{
	k_printf("t_write\n");
	return 20;
}

int t_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
	k_printf("ioctl\n");
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

	k_printf("run process2\n");

	int ret = fs_register_dev("/dev/process2", ops);
	if (ret < 0)
	{
		k_printf("reg err!\n");
		return;
	}

	int fd = open("/dev/process2", 0, 0);
	if (fd < 0)
	{
		k_printf("open err!\n");
		return;
	}

	char buff[100] = {0};
	for (int i = 0; i < 5; i++)
	{
		read(fd, buff, 100);
		sleep_ticks(500);

		write(fd, buff, 100);
		sleep_ticks(500);
	}

	ioctl(fd, 1, 2);
	close(fd);
}

int main(int argc, char **argv)
{
	//操作系统初始化，为TCB分配内存空间，同时启动空闲任务
	kernel_startup();
	//初始化标准输入输出设备
	stddev_init();
	//创建pcb资源清理进程
	pcb_clear_process();
	//系统时钟初始化，在系统中断服务程序中任务调度
	// pcb_create(0, task0_entry, NULL, STATCK_SIZE);
	// pcb_create(1, task1_entry, NULL, STATCK_SIZE);
	// pcb_create(2, task2_entry, NULL, STATCK_SIZE_LONG);
	pcb_create(3, task3_entry, NULL, STATCK_SIZE);

	//打开系统时钟，启动任务切换
	systick_init();

	while (1)
	{
	}
}
