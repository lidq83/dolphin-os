#include <typedef.h>
#include <led.h>
#include <timer.h>
#include <string.h>

#define SIZE (128)

//PendSV中断控制器地址
#define NVIC_INT_CTRL 0xE000Ed04
//触发PendSV
#define NVIC_PENDSV_SET 0x10000000
//PendSV优先级控制地址
#define NVIC_SYSPRI2 0xE000Ed22
//PendSV设置为最低优先值255
#define NVIC_PENDSV_PRI 0x000000ff

__IO uint32_t tick = 0;
__IO uint32_t p_ind = 0;
__IO uint32_t *p_stack[2] = {0};

uint32_t stack[2][SIZE] = {0};

void task0(void);
void task1(void);

uint32_t *stack_init(void *p_entry, uint32_t *stack)
{
	memset(stack, 0, sizeof(uint32_t) * SIZE);
	uint32_t *stk = stack;
	// uint32_t *stk = &stack[SIZE - 16];

	// stack[SIZE - 1] = 0x01000000uL;
	// stack[SIZE - 2] = (uint32_t)p_entry;

	// 初始化xPSP, PC, LR等寄存器
	*(--stk) = (uint32_t)0x01000000uL;
	*(--stk) = (uint32_t)p_entry;
	*(--stk) = (uint32_t)0; /* R14 (LR)                                           */
	*(--stk) = (uint32_t)0x12121212uL;
	*(--stk) = (uint32_t)0x03030303uL;
	*(--stk) = (uint32_t)0x02020202uL;
	*(--stk) = (uint32_t)0x01010101uL;
	*(--stk) = (uint32_t)stack;

	//初始化R11-R4
	*(--stk) = (uint32_t)0x11111111uL;
	*(--stk) = (uint32_t)0x10101010uL;
	*(--stk) = (uint32_t)0x09090909uL;
	*(--stk) = (uint32_t)0x08080808uL;
	*(--stk) = (uint32_t)0x07070707uL;
	*(--stk) = (uint32_t)0x06060606uL;
	*(--stk) = (uint32_t)0x05050505uL;
	*(--stk) = (uint32_t)0x04040404uL;

	return stk;
}

void RCC_config()
{
	ErrorStatus HSEStartUpStatus;

	RCC_DeInit();

	RCC_HSEConfig(RCC_HSE_ON);

	HSEStartUpStatus = RCC_WaitForHSEStartUp();

	if (HSEStartUpStatus == SUCCESS)
	{
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		RCC_PCLK1Config(RCC_HCLK_Div2);
		RCC_PCLK2Config(RCC_HCLK_Div1);
		RCC_PLLConfig(RCC_PLLSource_HSE_Div2, RCC_PLLMul_9);

		RCC_PLLCmd(ENABLE);
		while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
		{
		}

		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		while (RCC_GetSYSCLKSource() != 0x08)
		{
		}
	}
}

void SetPendSVPro(void)
{
	__ASM volatile("mov		r0,		%0 \n" ::"r"(NVIC_SYSPRI2));
	__ASM volatile("mov		r1,		%0 \n" ::"r"(NVIC_PENDSV_PRI));
	__ASM volatile("str		r1,		[r0] \n");
	// __ASM volatile("bx		lr");
}

void TriggerPendSV(void)
{
	__ASM volatile("mov		r0,		%0 \n" ::"r"(NVIC_INT_CTRL));
	__ASM volatile("mov		r1,		%0 \n" ::"r"(NVIC_PENDSV_SET));
	__ASM volatile("str		r1,		[r0] \n");
	//__ASM volatile("bx		lr");
}

void sche_first(void)
{
	__ASM volatile("mov	r0,	$0 \n");
	__ASM volatile("msr	psp, r0 \n");

	__ASM volatile("mov		r0,		%0 \n" ::"r"(NVIC_INT_CTRL));
	__ASM volatile("mov		r1,		%0 \n" ::"r"(NVIC_PENDSV_SET));
	__ASM volatile("str		r1,		[r0] \n");
}

void PendSV_Handler(void)
{
	static uint8_t is_first_sche = 1;
	uint32_t result = 0;

	__ASM volatile("CPSID   I \n");

	if (is_first_sche)
	{
		is_first_sche = 0;
		__ASM volatile("mov		r0, %0"::"r"(p_stack[p_ind % 2]));
		__ASM volatile("ldm	r0, {r4-r11}");
		__ASM volatile("adds	r0, r0, $0x20");
		__ASM volatile("msr	psp, r0");
	}
	else
	{
		__ASM volatile("mrs     r0, psp");
		__ASM volatile("subs    r0, r0, $0x20");
		__ASM volatile("stm   r0, {r4-r11}");		
		__ASM volatile("mov    %0, r0":"=r"(result));
		p_stack[(p_ind + 1) % 2] = (uint32_t *)result;	// 更新栈指针
		__ASM volatile("mov		r0, %0"::"r"(p_stack[p_ind % 2]));
		__ASM volatile("ldm	r0, {r4-r11} \n");
		__ASM volatile("adds	r0, r0, $0x20 \n");
		__ASM volatile("msr	psp, r0 \n");
	}
	p_ind++;
	__ASM volatile("ORR     LR, LR, #0x04 \n");

	__ASM volatile(" CPSIE   I \n");
}

void SysTick_Handler(void)
{
	if (tick++ % 1000 == 0)
	{
		TriggerPendSV();
	}
}

void task0(void)
{
	uint32_t i = 0;
	int j = 0;
	while (1)
	{
		if (i % 720000 == 0)
		{
			led_on(0);
		}
		if (i % 720000 == 360000)
		{
			led_off(0);
		}
		i++;
	}
}

void task1(void)
{
	uint32_t i = 0;
	int j = 0;
	while (1)
	{
		if (i % 720000 == 0)
		{
			led_on(1);
		}
		if (i % 720000 == 360000)
		{
			led_off(1);
		}
		i++;
	}
}

int main(int argc, char *argv[])
{
	SystemInit();

	RCC_config();

	led_init();

	p_stack[0] = stack_init(task0, &stack[0][SIZE]);
	p_stack[1] = stack_init(task1, &stack[1][SIZE]);

	SetPendSVPro();

	timer_init();

	timer_start();

	while (1)
	{
	}
}
