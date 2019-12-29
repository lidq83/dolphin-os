/*
 * mm.c
 *
 *  Created on: December 25, 2019
 *      Author: lidq
 */
#include <mm.h>
#include <sem.h>

//堆内存静态分配，其大小在各个芯片中有单独的定义
static uint8_t mm_heap[MM_SIZE_HEAP] = { 0 };

//内存块头指针
static mm_heap_s *p_head = NULL;

//当前游标指针
static mm_heap_s *p_cursor = NULL;

//内存申请信号量
static sem_t sem_rw = { 0 };

//实际内存分配函数
static void *mm_alloc(unsigned int size);

//实际内存释放函数
static void mm_free(void *addr);

//实际内存编译函数
static unsigned int mm_free_total(void);

//将一个内存块分割成两个子块
static mm_heap_s* mm_split(mm_heap_s *p, unsigned int size);

//设置当前内存块中最后的4个字节为指向当前块的指针
static int mm_set_self(mm_heap_s *p);

//获取当前内存块中最后4个字节的指针地址
static void* mm_self_pointer(mm_heap_s *p);

//将一个空内存块相邻的两个空块（前驱块和后继块）合并成为一个新空块
static mm_heap_s* mm_merge_both(mm_heap_s *p_pre, mm_heap_s *p_curr, mm_heap_s *p_next);

//将一个空内存块相邻的后继空块合并成为一个新空块
static mm_heap_s* mm_merge_next(mm_heap_s *p_curr, mm_heap_s *p_next);

//将一个空内存块相邻的前驱空块合并成为一个新空块
static mm_heap_s* mm_merge_pre(mm_heap_s *p_pre, mm_heap_s *p_curr);

//将一个内存块不相邻的前一个空闲块和后一个空闲块用双向指针连接在一起
static mm_heap_s* mm_link_both_addr(mm_heap_s *p_curr, void *addr_pre, void *addr_next);

//初始化内存堆
int mm_init(void)
{
	//初始化信号量
	sem_init(&sem_rw, 1);

	//初始化内存堆
	for (int i = 0; i < MM_SIZE_HEAP; i++)
	{
		mm_heap[i] = 0;
	}

	//内存块头节点
	p_head = (mm_heap_s*) &mm_heap[0];
	//前驱
	p_head->pre = NULL;
	//后继
	p_head->next = NULL;
	//标记为未使用
	p_head->use_flag = 0;
	//可用块大小，实际块大小 - 内存块头大小 - 最后一个4字节内存地址
	p_head->size = MM_SIZE_HEAP - sizeof(mm_heap_s) - sizeof(void*);
	//设置最后4字节指针指向当前块
	mm_set_self(p_head);
	//设置游标指针
	p_cursor = p_head;

	return 0;
}

//内存申请
void *malloc(unsigned int size)
{
	//等待信号量
	sem_wait(&sem_rw);
	//内存申请
	void *addr = mm_alloc(size);
	//释放信号量
	sem_post(&sem_rw);
	//返回内存地址
	return addr;
}

//释放内存地址
void free(void *addr)
{
	//等待信号量
	sem_wait(&sem_rw);
	//释放内存地址
	mm_free(addr);
	//释放信号量
	sem_post(&sem_rw);
}

//统计内存使用情况
unsigned int mallinfo(void)
{
	//等待信号量
	sem_wait(&sem_rw);
	//统计内存空闲总大小
	int size = mm_free_total();
	//释放信号量
	sem_post(&sem_rw);
	//返回空闲内存总大小
	return size;
}

//内存申请算法
void* mm_alloc(unsigned int size)
{
	//根据用户申请的内存做内存对齐，等到实际申请内存大小，避免内存碎片
	unsigned int alloc_size = MM_ALIGN_UP(size + SIZEOF_MM_ALLOCNODE);
	//最终申请的内存地址
	void *addr = NULL;
	//申请到的内存块
	mm_heap_s *p_ret = NULL;
	//获取内存块游标
	mm_heap_s *p = p_cursor;
	if (p == NULL)
	{
		p = p_head;
	}

	//在内存块中循环查找合适的空闲块
	while (1)
	{
		//如果当前块为空闲块则进行后续大小判断处理
		if (!p->use_flag)
		{
			//尝试将当前块根据申请内存的大小分割成两个小块
			mm_heap_s *mm_new = mm_split(p, alloc_size);
			//如果分割小块成功，则内存申请成功
			if (mm_new != NULL)
			{
				//设置游标
				p_cursor = p;
				//设置返回内存块
				p_ret = p;
				//设置新内存地址
				p = mm_new;
				//跳转到申请成功标签
				goto _found;
			}

			//如果分割内存块失败，则判断当前空闲块的可用大小时否满足申请内存大小
			//如果满足，则将当前空闲块直接分配给用户
			if (alloc_size + sizeof(mm_heap_s) + sizeof(void*) <= p->size)
			{
				//设置游标
				p_cursor = p;
				//设置返回内存块
				p_ret = p;
				//设置p为下一个节点
				p = p->next;
				//跳转到申请成功标签
				goto _found;
			}
		}

		//如果内存块链表结束，则从头开始
		if (p->next == NULL)
		{
			//从头再来
			p = p_head;
		}
		else
		{
			//移动指针到下一个节点
			p = p->next;
		}

		//如果所有节点都已经遍历，且没有找到合适的空闲块
		if (p == p_cursor)
		{
			//返回内存申请失败标签
			goto _not_found;
		}
	}

	////如果内存块链表结束
	if (p->next == NULL)
	{
		//返回内存申请失败标签
		goto _not_found;
	}

//申请内存成功
_found: ;
	//设置分配块使用标记为已使用
	p_ret->use_flag = 1;
	//设置返回地址为当前块地址
	addr = p_ret;
	//返回地址增加块头大小，即用户实际可用地址
	addr += sizeof(mm_heap_s);
	//回返内存地址
	return addr;

//申请内存失败
_not_found: ;
	return NULL;
}

//内存释放，合并相邻的空闲块，有效避免内存碎块残留
void mm_free(void *addr)
{
	//计算用户释放的内存块所在地址：用户使用地址 - 块头大小
	mm_heap_s *p_curr = addr - sizeof(mm_heap_s);
	//最终合并的内存块
	mm_heap_s *p_result = p_curr;
	//整个堆内存的起始地址
	void *addr_min = (void*) &mm_heap[0];
	//整个堆内存的结束地址
	void *addr_max = (void*) &mm_heap[MM_SIZE_HEAP];
	//后继内存块地址
	void *addr_next = (void *) p_curr + sizeof(mm_heap_s) + p_curr->size + sizeof(void*);
	//前驱内存块地址（注意，这里是与当前内存志相邻的最后4字节指针地址，不是前驱内存块的地址）
	void *addr_pre = (void*) p_curr - sizeof(void *);

	//如果前驱内存块与后继内存块都是有效的
	if (addr_pre >= addr_min && addr_next < addr_max)
	{
		//得到后继内存块
		mm_heap_s* p_next = (mm_heap_s *) addr_next;
		//得到前驱内存块
		mm_heap_s* p_pre = *(mm_heap_s **) addr_pre;
		//如果前驱内存块和后继内存块都是空闲块
		if (!p_next->use_flag && !p_pre->use_flag)
		{
			//将当前块和其相邻的前驱块和后继块合并为一个新块
			p_result = mm_merge_both(p_pre, p_curr, p_next);
			//跳转到释放成功标签
			goto _free_success;
		}
	}

	//如果后继块地址有效
	if (addr_next < addr_max)
	{
		//得到后续内存块
		mm_heap_s* p_next = (mm_heap_s *) addr_next;
		//如果后继块是一个空闲块（前驱块不是空闲块）
		if (!p_next->use_flag)
		{
			//将当前块与后继块合并为一个新块
			p_result = mm_merge_next(p_curr, p_next);
			//跳转到释放成功标签
			goto _free_success;
		}
	}

	//如果前驱块地址有效
	if (addr_pre >= addr_min)
	{
		//得到前驱内存块
		mm_heap_s* p_pre = *(mm_heap_s **) addr_pre;
		//如果前驱块是一个空闲块（后继块不是空闲块）
		if (!p_pre->use_flag)
		{
			//将当前块与后继块合并为一个新块
			p_result = mm_merge_pre(p_pre, p_curr);
			//跳转到释放成功标签
			goto _free_success;
		}
	}

	//如果前驱地址有效
	if (addr_pre >= addr_min)
	{
		//得到前驱内存块的实际地址
		mm_heap_s* p_pre = *(mm_heap_s **) addr_pre;
		addr_pre = p_pre;
	}
	//如果前驱地址无效，则设置其为空
	else
	{
		addr_pre = NULL;
	}

	//如果后继地址有效
	if (addr_next < addr_max)
	{
		//得到后继内存志的实地地址
		mm_heap_s* p_next = (mm_heap_s *) addr_next;
		addr_next = p_next;
	}
	//如果后继地址无效，则设置其为空
	else
	{
		addr_next = NULL;
	}

	//将一个内存块不相邻的前一个空闲块和后一个空闲块用双向指针连接在一起
	p_result = mm_link_both_addr(p_curr, addr_pre, addr_next);

	//释放内存成功
	_free_success: ;

	//设置最终新内存块为未使用
	p_result->use_flag = 0;
	//设置当前块的自身指针
	mm_set_self(p_result);
	//更新游标
	p_cursor = p_result;
}

//统计内存使用情况，空闲内存统计
unsigned int mm_free_total(void)
{
	//从头节点开始
	mm_heap_s *p = p_head;
	//空闲内存总大小
	unsigned int size = 0;
	//循环统计
	while (p)
	{
		//如果内存块为未使用
		if (!p->use_flag)
		{
			//计算空闲内存总大小
			size += p->size;
		}
		//移动指针到下一个节点
		p = p->next;
	}
	//返回空闲内存总大小
	return size;
}

//将一个内存块分割成两个子块
mm_heap_s* mm_split(mm_heap_s *p, unsigned int size)
{
	//如果需要分割的内存不足，则直接返回NULL,表示分割失败
	//计算方式：第1块内存头 + 第1块大小 + 第1块最后4字节指针 + 第2块内存头 + 第2块大小 + 第2块最后4字节指针
	if (p->size <= 2 * sizeof(mm_heap_s) + 2 * sizeof(void*) + size)
	{
		return NULL;
	}
	//申请内存地址
	void *addr = p;
	//返回块
	mm_heap_s *ret = addr + sizeof(mm_heap_s) + sizeof(void*) + size;
	//返回块的前驱为p
	ret->pre = p;
	//返回块的后继为p的后继
	ret->next = p->next;
	//计算返回块的大小
	ret->size = p->size - size - sizeof(mm_heap_s) - sizeof(void*);
	//使用使用标记为空闲
	ret->use_flag = 0;

	//设置新内存块最后4字节指针
	mm_set_self(ret);

	//设置p的后继为新块
	p->next = ret;
	//设置p的可用大小
	p->size = size;

	//设置p的最后4字节指针
	mm_set_self(p);

	return ret;
}

//设置当前内存块中最后的4个字节为指向当前块的指针
int mm_set_self(mm_heap_s *p)
{
	//设置内存块最后4字节地址为当前块内存地址
	mm_heap_s **self = (mm_heap_s**) mm_self_pointer(p);
	*self = p;
	return 0;
}

//获取当前内存块中最后4个字节的指针地址
void* mm_self_pointer(mm_heap_s *p)
{
	//当前块地址
	void *addr = p;
	//计算最后4字节位置
	addr += sizeof(mm_heap_s) + p->size;
	//返回最后4字节地址
	return addr;
}

//将一个空内存块相邻的两个空块（前驱块和后继块）合并成为一个新空块
mm_heap_s* mm_merge_both(mm_heap_s *p_pre, mm_heap_s *p_curr, mm_heap_s *p_next)
{
	//前驱块的后继 = 后继块的后继
	p_pre->next = p_next->next;
	//如果后继块的后继不为空
	if (p_next->next != NULL)
	{
		//后继块的后继的前驱 = 前驱
		p_next->next->pre = p_pre;
	}
	//计算可用大小
	p_pre->size += p_curr->size + p_next->size + 2 * sizeof(mm_heap_s) + 2 * sizeof(void *);
	//合并完成后，前驱即为新的内存块
	return p_pre;
}

//将一个空内存块相邻的后继空块合并成为一个新空块
mm_heap_s* mm_merge_next(mm_heap_s *p_curr, mm_heap_s *p_next)
{
	//当前块的后继 = 后继块的后继
	p_curr->next = p_next->next;
	//如果后继块的后继不为空
	if (p_next->next != NULL)
	{
		//后继块的后继的前驱 = 当前块
		p_next->next->pre = p_curr;
	}
	//计算可用大小
	p_curr->size += p_next->size + sizeof(mm_heap_s) + sizeof(void *);
	//合并完成后，当前块即为新的内存块
	return p_curr;
}

//将一个空内存块相邻的前驱空块合并成为一个新空块
mm_heap_s* mm_merge_pre(mm_heap_s *p_pre, mm_heap_s *p_curr)
{
	//前驱块的后继 = 当前块的后继
	p_pre->next = p_curr->next;
	//如果当前块的后继不为空
	if (p_curr->next != NULL)
	{
		//当前块的后继的前驱 = 前驱块
		p_curr->next->pre = p_pre;
	}
	//计算可用大小
	p_pre->size += sizeof(mm_heap_s) + p_curr->size + sizeof(void *);
	//合并完成后，前驱块为新的内存志
	return p_pre;
}

//将一个内存块不相邻的前一个空闲块和后一个空闲块用双向指针连接在一起
mm_heap_s* mm_link_both_addr(mm_heap_s *p_curr, void *addr_pre, void *addr_next)
{
	//无需处理当前块的前驱指针和后继指针，已经通过链表将其连接在一起了。
	return p_curr;
}

