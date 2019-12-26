/*
 * mm.h
 *
 *  Created on: December 25, 2019
 *      Author: lidq
 */
#ifndef __SRC_KERNEL_MM_H
#define __SRC_KERNEL_MM_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <defs.h>


#define MM_MIN_SHIFT            4
#define MM_MIN_CHUNK            (1 << MM_MIN_SHIFT)
#define MM_MIN_ALIGN_MASK       (MM_MIN_CHUNK-1)
#define MM_ALIGN_UP(size)       (((size) + MM_MIN_ALIGN_MASK) & ~MM_MIN_ALIGN_MASK)

#define SIZEOF_MM_ALLOCNODE     8

typedef struct mm_heap_s
{
	struct mm_heap_s *pre;
	struct mm_heap_s *next;
	unsigned char use_flag;
	unsigned int size;
} mm_heap_s;

int mm_init(void);

void *malloc(unsigned int size);

void free(void *addr);

unsigned int mallinfo(void);

#endif
