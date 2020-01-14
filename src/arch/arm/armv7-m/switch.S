
.extern pcb_current
.extern pcb_next

.global  sche_interrupt_disable                                     
.global  sche_interrupt_enable
.global  sche_switch_first
.global  sche_switch_nosave
.global  sche_switch
.global  PendSV_Handler
.global  PendSVHandler_nosave


 	.cpu    cortex-m3
    .fpu    softvfp
    .syntax unified
    .thumb
    .text

@PendSV中断控制器地址
.equ  NVIC_INT_CTRL,      0xE000Ed04 
@触发PendSV
.equ  NVIC_PENDSV_SET,    0x10000000 
@PendSV优先级控制地址
.equ  NVIC_SYSPRI2,       0xE000Ed22 
@PendSV设置为最低优先值255
.equ  NVIC_PENDSV_PRI,    0x000000ff 


	.global sche_interrupt_disable
    .type sche_interrupt_disable, %function
sche_interrupt_disable:
    MRS     R0, PRIMASK
    CPSID   I
	BX      LR

	.global sche_interrupt_enable
    .type sche_interrupt_enable, %function
sche_interrupt_enable:
    MSR     PRIMASK, R0
	BX      LR

	.global sche_switch_first
    .type sche_switch_first, %function
sche_switch_first:
	@设置中断优先级为最低
	LDR     R0, =NVIC_SYSPRI2                                 
    LDR     R1, =NVIC_PENDSV_PRI
    STRB    R1, [R0]
    
	@PSP = 0
    MOVS    R0, #0                                             
    MSR     PSP, R0
	
	@触发pendsv异常
	LDR     R0, =NVIC_INT_CTRL                                  
    LDR     R1, =NVIC_PENDSV_SET
    STR     R1, [R0]
	   
	BX LR
	
	@防止编译出现警告信息
	NOP

	.global sche_switch_nosave
    .type sche_switch_nosave, %function
sche_switch_nosave:
	@PSP = 0
    MOVS    R0, #0                                             
    MSR     PSP, R0
	
	@触发pendsv异常
	LDR     R0, =NVIC_INT_CTRL                                  
    LDR     R1, =NVIC_PENDSV_SET
    STR     R1, [R0]
	   
	BX LR
	
	@防止编译出现警告信息
	NOP

	.global sche_switch
    .type sche_switch, %function
sche_switch:
	@触发pensv异常
    LDR     R0, =NVIC_INT_CTRL                              
    LDR     R1, =NVIC_PENDSV_SET
    STR     R1, [R0] 
    BX      LR
	
	@防止编译出现警告信息
	NOP

	.global PendSV_Handler
    .type PendSV_Handler, %function
PendSV_Handler:
	@关闭中断
	CPSID   I
	
    @PSP => RO	
    MRS     R0, PSP 
    
    @初始化ygos的时候会初始化PSP为0
    CMP     R0, #0	
    BEQ     PendSVHandler_nosave                  
    
	@通过寄存器R4-R11，一共是8个字的寄存器，占用空间是4*8=32byte
    SUBS    R0, R0, #0x20   
    @保存寄存器R4-R11到PSP的地址	
    STM     R0, {R4-R11}
    
	@加载ygos_tcb_current指针到R1
    LDR     R1, =pcb_current 
     @加载R1的堆栈指针	
    LDR     R1, [R1]
	@存储R0寄存器的值,到堆栈指针的位置
    STR     R0, [R1]                                          

PendSVHandler_nosave:
	@pcb_current = pcb_next
    LDR     R0, =pcb_current                                       
    LDR     R1, =pcb_next
    LDR     R2, [R1]
    STR     R2, [R0]
    
	@R0加载
    LDR     R0, [R2]
     @R4-R11的8个寄存器恢复到pcb_current的堆栈地址	
    LDM     R0, {R4-R11} 
    @更新栈地址的指针到R0	
    ADDS    R0, R0, #0x20
    
	@更新R0到PSP
    MSR     PSP, R0 
     @栈地址为用户栈指针PSP
    ORR     LR, LR, #0x04    
    
    @打开中断	
    CPSIE   I
    BX      LR  
	
	@防止编译出现警告信息
    NOP	 