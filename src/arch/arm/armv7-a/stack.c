#include <stdint.h>

#define USERMODE 0x10
#define FIQMODE 0x11
#define IRQMODE 0x12
#define SVCMODE 0x13
#define MONITORMODE 0x16
#define ABORTMODE 0x17
#define HYPMODE 0x1b
#define UNDEFMODE 0x1b
#define MODEMASK 0x1f
#define NOINT 0xc0

//初始化进程栈
void *stack_init(uint32_t *stack, void *runner)
{
	uint32_t *stk = stack;

	*(stk) = (uint32_t)runner; /* entry point */
	*(--stk) = (uint32_t)0x0;  /* lr */
	*(--stk) = 0;			   /* r12 */
	*(--stk) = 0;			   /* r11 */
	*(--stk) = 0;			   /* r10 */
	*(--stk) = 0;			   /* r9 */
	*(--stk) = 0;			   /* r8 */
	*(--stk) = 0;			   /* r7 */
	*(--stk) = 0;			   /* r6 */
	*(--stk) = 0;			   /* r5 */
	*(--stk) = 0;			   /* r4 */
	*(--stk) = 0;			   /* r3 */
	*(--stk) = 0;			   /* r2 */
	*(--stk) = 0;			   /* r1 */
	*(--stk) = 0;			   /* r0 : argument */

	/* cpsr */
	if ((uint32_t)runner & 0x01)
	{
		*(--stk) = SVCMODE | 0x20; /* thumb mode */
	}
	else
	{
		*(--stk) = SVCMODE; /* arm mode   */
	}
	return stk;
}