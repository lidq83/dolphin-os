# Dolphin-OS

Dolphin-OS是一款嵌入式的实时操作系统。她支持并行执行多任务，信号量机制。

## 硬件平台

- stm32f1discovery
- stm32f4discovery

## 开发平台

- 系统平台：Linux
- 编译工具：arm-none-eabi-gcc
- 构建工具：Makefile
- 烧录工具：JLink

## 功能说明

- 进程管理支持最多32个进程，即最多32个进程控制块PCB。
- 系统采用优先级抢占式调度，优先级由0～31依次降低。
- 支持进程通过sleep_ticks(int tick)函数主动休眠。
- 支持信号量Semaphore机制，实现优先级反转功能。
