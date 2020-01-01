#include <core.h>
#include <pcb.h>
#include <sche.h>
#include "debug.h"
#include "systick.h"

#define DEBUG_LOG printf_str

#define TASK1_STATCK_SIZE 128
#define TASK2_STATCK_SIZE 128

//任务1和任务2的堆栈
uint8_t task1_stack[TASK1_STATCK_SIZE];
uint8_t task2_stack[TASK2_STATCK_SIZE];

//任务
int task1_flag = 0;
int task2_flag = 0;

//任务1入口函数
void task1_entry(void *arg)
{
	while (1)
	{
		//task1_flag变量200个tick进行翻转
		task1_flag = 1;
		sleep_ticks(200);
		task1_flag = 0;
		sleep_ticks(200);
		DEBUG_LOG("run thread1\n");
	}
}

//任务2入口函数
void task2_entry(void *arg)
{
	while (1)
	{
		//task2_flag变量200个tick进行翻转
		task2_flag = 1;
		sleep_ticks(200);
		task2_flag = 0;
		sleep_ticks(200);
		DEBUG_LOG("run thread2\n");
	}
}

int main(int argc, char **argv)
{
	DEBUG_LOG("Hello ygOS\n");

	//操作系统初始化，为TCB分配内存空间，同时启动空闲任务
	//ygos_init();
	kernel_startup();

	//系统时钟初始化，在系统中断服务程序中任务调度
	systick_init();

	//创建用户任务
	pcb_create(0, task1_entry, (void *)0, &task1_stack[TASK1_STATCK_SIZE]);
	pcb_create(1, task2_entry, (void *)0, &task2_stack[TASK2_STATCK_SIZE]);

	//操作系统启动，会触发任务调度到最高优先级的任务
	//ygos_start();

	// //ygos启动失败,会执行下面
	while (1)
	{
		//return -1;
	}
}
