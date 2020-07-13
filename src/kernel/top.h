#ifndef __TOP_H_
#define __TOP_H_

#include <stdint.h>
#include <pcb.h>

typedef struct top_s
{
	uint8_t prio;
	uint32_t stack_size;
	uint32_t stack_free;
	uint32_t stack_used;
} top_s;

uint8_t top(top_s *t);

#endif