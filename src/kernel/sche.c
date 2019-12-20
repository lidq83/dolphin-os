/*
 * sche.c
 *
 *  Created on: December 20, 2019
 *      Author: lidq
 */

#include <sche.h>
#include <pcb.h>

//PendSV中断控制器地址
#define NVIC_INT_CTRL 0xE000Ed04
//触发PendSV
#define NVIC_PENDSV_SET 0x10000000
//PendSV优先级控制地址
#define NVIC_SYSPRI2 0xE000Ed22
//PendSV设置为最低优先值255
#define NVIC_PENDSV_PRI 0x000000ff

static pcb_s *p_current;
static pcb_s *p_next;

void set_pend_sv_proi(void)
{
	__ASM volatile("mov		r0,		%0" ::"r"(NVIC_SYSPRI2));
	__ASM volatile("mov		r1,		%0" ::"r"(NVIC_PENDSV_PRI));
	__ASM volatile("str		r1,		[r0]\n");
}

void trigger_pend_sv(void)
{
	__ASM volatile("mov		r0,		%0" ::"r"(NVIC_INT_CTRL));
	__ASM volatile("mov		r1,		%0" ::"r"(NVIC_PENDSV_SET));
	__ASM volatile("str		r1,		[r0]");
}

void PendSV_Handler(void)
{
	static uint8_t is_first_sche = 1;
	uint32_t result = 0;

	__ASM volatile("cpsid       I \n");

	if (is_first_sche)
	{
		is_first_sche = 0;
		goto _no_save;
	}

    __ASM volatile("mrs     r0,     psp");
    __ASM volatile("subs    r0,     r0, $0x20");
    __ASM volatile("stm     r0,     {r4-r11}");		
    __ASM volatile("mov     %0,     r0":"=r"(result));
    p_current->p_stack = (void *)result;
    p_current = p_next;

_no_save:
    __ASM volatile("mov     r0,     %0"::"r"(p_current->p_stack));
    __ASM volatile("ldm     r0,     {r4-r11}");
    __ASM volatile("adds    r0,     r0, $0x20");
    __ASM volatile("msr     psp,    r0");

	__ASM volatile("orr         lr,     lr, #0x04");
	__ASM volatile("cpsie       I \n");
}