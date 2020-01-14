#include <task.h>
#include <core.h>

int main(int argc, char **argv)
{
	kernel_startup();

	pcb_clear_process();

	task_examples();

	sche_switch();

	while (1)
	{
	}
}
