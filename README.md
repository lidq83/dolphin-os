# Dolphin-OS

Dolphin-OS是一款嵌入式的多任务、抢占式、实时操作系统。

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
- 支持malloc()和free()函数动态申请和释放内存。
- 采用伙伴算法完成内存的分配与回收。
- 整理并合并内存碎块,避免内存碎片无法重复使用。
- 支持虚拟文件系统，可由用户注册和删除设备节点。
- 支持POSIX文件操作标准(open、close、read、write、ioctl)。
- 支持QEMU仿真模拟，完成printf格式化输出函数。
- 支持stdin、stdout和stderr设备节点。
- 支持pcb进程运行后自动终止和运行中动态创建。
- 支持pcb进程内存清理，回收终止的pcb占用的内存资源。
- 支持STM32F4xx系列处理器。
- 支持串口设备做为标准IO设备。
- 在不同的开发板的board.h中定义不同的内存资源。
- 支持snprintf()和strtod()等函数功能，方便对数字和字符串的转化。
