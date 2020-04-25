#include <stdint.h>

//初始化进程栈
void *stack_init(uint32_t *stack, void *runner)
{
	uint32_t *stk = stack;

	// 初始化xPSR，PC，LR，R12，R0-R3等寄存器
	*(--stk) = (uint32_t)0x01000000uL;
	*(--stk) = (uint32_t)runner;
	*(--stk) = (uint32_t)0x0uL;
	*(--stk) = (uint32_t)0x0uL;
	*(--stk) = (uint32_t)0x0uL;
	*(--stk) = (uint32_t)0x0uL;
	*(--stk) = (uint32_t)0x0uL;
	*(--stk) = (uint32_t)0x0uL;

	//初始化R11-R4
	*(--stk) = (uint32_t)0x0uL;
	*(--stk) = (uint32_t)0x0uL;
	*(--stk) = (uint32_t)0x0uL;
	*(--stk) = (uint32_t)0x0uL;
	*(--stk) = (uint32_t)0x0uL;
	*(--stk) = (uint32_t)0x0uL;
	*(--stk) = (uint32_t)0x0uL;
	*(--stk) = (uint32_t)0x0uL;

	return stk;
}
