#ifndef __DEFS_H
#define __DEFS_H

//堆内存大小42KB，为其它全局变量预留4KB
//如全局变量内存空闲不足，则可以适当减小堆内存大小）
#define MM_SIZE_HEAP    (1024 * 42)

#endif
