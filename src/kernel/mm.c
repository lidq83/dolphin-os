/*
 * mm.c
 *
 *  Created on: December 25, 2019
 *      Author: lidq
 */
#include <mm.h>
#include <sem.h>

static uint8_t mm_heap[MM_SIZE_HEAP] = { 0 };

static mm_heap_s *p_head = NULL;

static mm_heap_s *p_cursor = NULL;

static sem_t sem_rw = { 0 };

static void *mm_alloc(unsigned int size);

static void mm_free(void *addr);

static unsigned int mm_free_total(void);

static mm_heap_s* mm_split(mm_heap_s *p, unsigned int size);

static int mm_set_self(mm_heap_s *p);

static void* mm_self_pointer(mm_heap_s *p);

static mm_heap_s* mm_merge_both(mm_heap_s *p_pre, mm_heap_s *p_curr, mm_heap_s *p_next);

static mm_heap_s* mm_merge_next(mm_heap_s *p_curr, mm_heap_s *p_next);

static mm_heap_s* mm_merge_pre(mm_heap_s *p_pre, mm_heap_s *p_curr);

static mm_heap_s* mm_link_both_addr(mm_heap_s *p_curr, void *addr_pre, void *addr_next);

int mm_init(void)
{
	sem_init(&sem_rw, 1);

	for (int i = 0; i < MM_SIZE_HEAP; i++)
	{
		mm_heap[i] = 0;
	}

	p_head = (mm_heap_s*) &mm_heap[0];
	p_head->pre = NULL;
	p_head->next = NULL;
	p_head->use_flag = 0;
	p_head->size = MM_SIZE_HEAP - sizeof(mm_heap_s) - sizeof(void*);

	mm_set_self(p_head);

	p_cursor = p_head;

	return 0;
}

void *malloc(unsigned int size)
{
	sem_wait(&sem_rw);
	void *addr = mm_alloc(size);
	sem_post(&sem_rw);
	return addr;
}

void free(void *addr)
{
	sem_wait(&sem_rw);
	mm_free(addr);
	sem_post(&sem_rw);
}

unsigned int mallinfo(void)
{
	sem_wait(&sem_rw);
	int size = mm_free_total();
	sem_post(&sem_rw);
	return size;
}

void* mm_alloc(unsigned int size)
{
	unsigned int alloc_size = MM_ALIGN_UP(size + SIZEOF_MM_ALLOCNODE);
	void *addr = NULL;
	mm_heap_s *p_ret = NULL;
	mm_heap_s *p = p_cursor;
	if (p == NULL)
	{
		p = p_head;
	}

	while (1)
	{
		if (!p->use_flag)
		{
			mm_heap_s *mm_new = mm_split(p, alloc_size);
			if (mm_new != NULL)
			{
				p_cursor = p;
				p_ret = p;
				p = mm_new;
				goto _found;
			}

			if (alloc_size + sizeof(mm_heap_s) + sizeof(void*) <= p->size)
			{
				p_cursor = p;
				p_ret = p;
				p = p->next;
				goto _found;
			}
		}

		if (p->next == NULL)
		{
			p = p_head;
		}
		else
		{
			p = p->next;
		}

		if (p == p_cursor)
		{
			goto _not_found;
		}
	}

	if (p->next == NULL)
	{
		goto _not_found;
	}

	_found: ;
	p_ret->use_flag = 1;
	addr = p_ret;
	addr += sizeof(mm_heap_s);
	return addr;

	_not_found: ;
	return NULL;
}

void mm_free(void *addr)
{
	mm_heap_s *p_curr = addr - sizeof(mm_heap_s);
	mm_heap_s *p_result = p_curr;

	void *addr_min = (void*) &mm_heap[0];
	void *addr_max = (void*) &mm_heap[MM_SIZE_HEAP];

	void *addr_next = (void *) p_curr + sizeof(mm_heap_s) + p_curr->size + sizeof(void*);
	void *addr_pre = (void*) p_curr - sizeof(void *);

	if (addr_pre >= addr_min && addr_next < addr_max)
	{
		mm_heap_s* p_next = (mm_heap_s *) addr_next;
		mm_heap_s* p_pre = *(mm_heap_s **) addr_pre;

		if (!p_next->use_flag && !p_pre->use_flag)
		{
			p_result = mm_merge_both(p_pre, p_curr, p_next);
			goto _free_success;
		}
	}

	if (addr_next < addr_max)
	{
		mm_heap_s* p_next = (mm_heap_s *) addr_next;

		if (!p_next->use_flag)
		{
			p_result = mm_merge_next(p_curr, p_next);
			goto _free_success;
		}
	}

	if (addr_pre >= addr_min)
	{
		mm_heap_s* p_pre = *(mm_heap_s **) addr_pre;

		if (!p_pre->use_flag)
		{
			p_result = mm_merge_pre(p_pre, p_curr);
			goto _free_success;
		}
	}

	if (addr_pre >= addr_min)
	{
		mm_heap_s* p_pre = *(mm_heap_s **) addr_pre;
		addr_pre = p_pre;
	}
	else
	{
		addr_pre = NULL;
	}

	if (addr_next < addr_max)
	{
		mm_heap_s* p_next = (mm_heap_s *) addr_next;
		addr_next = p_next;
	}
	else
	{
		addr_next = NULL;
	}
	p_result = mm_link_both_addr(p_curr, addr_pre, addr_next);

	_free_success: ;

	p_result->use_flag = 0;
	mm_set_self(p_result);
	p_cursor = p_result;
}

unsigned int mm_free_total(void)
{
	mm_heap_s *p = p_head;
	unsigned int size = 0;
	while (p)
	{
		if (!p->use_flag)
		{
			size += p->size;
		}
		p = p->next;
	}
	return size;
}

mm_heap_s* mm_split(mm_heap_s *p, unsigned int size)
{
	if (p->size <= 2 * sizeof(mm_heap_s) + 2 * sizeof(void*) + size)
	{
		return NULL;
	}

	void *addr = p;
	mm_heap_s *ret = addr + sizeof(mm_heap_s) + sizeof(void*) + size;
	ret->pre = p;
	ret->next = p->next;
	ret->size = p->size - size - sizeof(mm_heap_s) - sizeof(void*);
	ret->use_flag = 0;

	mm_set_self(ret);

	p->next = ret;
	p->size = size;

	mm_set_self(p);

	return ret;
}

int mm_set_self(mm_heap_s *p)
{
	mm_heap_s **self = (mm_heap_s**) mm_self_pointer(p);
	*self = p;

	return 0;
}

void* mm_self_pointer(mm_heap_s *p)
{
	void *addr = p;
	addr += sizeof(mm_heap_s) + p->size;
	return addr;
}

mm_heap_s* mm_merge_both(mm_heap_s *p_pre, mm_heap_s *p_curr, mm_heap_s *p_next)
{
	p_pre->next = p_next->next;
	if (p_next->next != NULL)
	{
		p_next->next->pre = p_pre;
	}
	p_pre->size += p_curr->size + p_next->size + 2 * sizeof(mm_heap_s) + 2 * sizeof(void *);

	return p_pre;
}

mm_heap_s* mm_merge_next(mm_heap_s *p_curr, mm_heap_s *p_next)
{
	p_curr->next = p_next->next;
	if (p_next->next != NULL)
	{
		p_next->next->pre = p_curr;
	}
	p_curr->size += p_next->size + sizeof(mm_heap_s) + sizeof(void *);

	return p_curr;
}

mm_heap_s* mm_merge_pre(mm_heap_s *p_pre, mm_heap_s *p_curr)
{
	p_pre->next = p_curr->next;
	if (p_curr->next != NULL)
	{
		p_curr->next->pre = p_pre;
	}
	p_pre->size += sizeof(mm_heap_s) + p_curr->size + sizeof(void *);

	return p_pre;
}

mm_heap_s* mm_link_both_addr(mm_heap_s *p_curr, void *addr_pre, void *addr_next)
{
	return p_curr;
}

