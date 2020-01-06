/*
 * mm.h
 *
 *  Created on: December 25, 2019
 *      Author: lidq
 */
#ifndef __SRC_MM_MM_H
#define __SRC_MM_MM_H

#include <stdio.h>
#include <stdint.h>
#include <k_string.h>

#ifndef MM_SIZE_HEAP
#define MM_SIZE_HEAP    (1024 * 32)
#endif 

#define MM_MIN_SHIFT            4
#define MM_MIN_CHUNK            (1 << MM_MIN_SHIFT)
#define MM_MIN_ALIGN_MASK       (MM_MIN_CHUNK-1)
#define MM_ALIGN_UP(size)       (((size) + MM_MIN_ALIGN_MASK) & ~MM_MIN_ALIGN_MASK)

//8字节对齐
#define SIZEOF_MM_ALLOCNODE     8

//内存块实际结构如下
//block[0]: |----pre----|---next----|-use_flag-|-malloc return memory address-----|-self pointer-|
//block[1]: |----pre----|---next----|-use_flag-|-malloc return memory address-----|-self pointer-|
//block[2]: |----pre----|---next----|-use_flag-|-malloc return memory address-----|-self pointer-|
//......  : ......
//block[n]: |----pre----|---next----|-use_flag-|-malloc return memory address-----|-self pointer-|

//内存块结构
typedef struct mm_heap_s
{
	//内存块前驱指针
	struct mm_heap_s *pre;
	//内存块后继指针
	struct mm_heap_s *next;
	//使用标识
	unsigned char use_flag;
	//内存块可用大小
	unsigned int size;
} mm_heap_s;

//初始化内存堆
int mm_init(void);

//动态内存申请
void *malloc(unsigned int size);

//内存释放
void free(void *addr);

//内存统计
unsigned int mallinfo(void);

#endif
