.extern pcb_current
.extern pcb_next

.section .text, "ax"

	.global sche_interrupt_disable
    .type sche_interrupt_disable, %function
sche_interrupt_disable:
    mrs r0, cpsr
    cpsid i
    bx  lr

@   r0 == level: level传递参数到r0, 为了防止关闭中断嵌套,保证最后
@   打开中断的时候, 进行打开中断, 所以在第一次关闭中断前的CPSR寄存器
@   是打开中断的值, 比如打开中断是1, 关闭中断0, 最后一次把1恢复到CPSR
@   才是正确的
	.global sche_interrupt_enable
    .type sche_interrupt_enable, %function
sche_interrupt_enable:
    msr cpsr, r0
    bx  lr

	.global sche_switch_first
    .type sche_switch_first, %function
sche_switch_first:
	bl context_switch

.section .text.isr, "ax"
.global sche_switch
sche_switch:

    @ 压栈lr代替压入pc指针
    stmfd   sp!, {lr}      
    stmfd   sp!, {r0-r12, lr}  

    @ CPSR的T=0：arm，T=1：thumb
    mrs r4, cpsr
    tst lr, #0x01
    orrne r4, r4, #0x20     
    stmfd sp!, {r4}         

    ldr     R3, =pcb_current                                     
    ldr r0, [r3]            
    str sp, [r0]           
    
    @ pcb_current = pcb_next
    ldr     R0, =pcb_current                                     
    ldr     R1, =pcb_next
    ldrb    R2, [R1]
    strb    R2, [R0]

 context_switch:
    ldr     R4, =pcb_next                                     
    ldr r1, [r4]
    ldr sp, [r1]           
    ldmfd sp!, {r4}         
    msr spsr_cxsf, r4
    ldmfd sp!, {r0-r12, lr, pc}^ 
