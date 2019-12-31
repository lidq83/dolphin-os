/*
 * slist.c
 *
 *  Created on: December 23, 2019
 *      Author: lidq
 */

#include "slist.h"

//第一个空闲位置
const uint8_t map_first_one[256] = {
	/* 00 */ 0, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* 10 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* 20 */ 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* 30 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* 40 */ 6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* 50 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* 60 */ 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* 70 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* 80 */ 7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* 90 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* A0 */ 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* B0 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* C0 */ 6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* D0 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* E0 */ 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
	/* F0 */ 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0
	/* END */
};

//初始化链表
int slist_init(slist_s *list)
{   
	if (list == NULL)
	{
		return -1;
	}

	memset(list, 0, sizeof(slist_s));
	//初始化位图，所有位置均可用
	list->use_map = ~0x1;
	return 0;
}

//追加节点到链表
int slist_append(slist_s *list, void *key, void *value)
{
	if (list == NULL)
	{
		return -1;
	}

	if (list->size >= SLIST_SIZE)
	{
		return -1;
	}

	//取得第一个空闲位置的索引号
	uint32_t ind = slist_first_empty(list);
	if (ind >= SLIST_SIZE)
	{
		return -1;
	}
	
	//申请节点
	slist_alloc(list, ind);
	//设置新节点的值
	slist_node_s *node_new = &list->node_map[ind];
	//索引号
	node_new->ind = ind;
	//关键字
	node_new->key = key;
	//值
	node_new->value = value;
	//下一个节点
	node_new->next = NULL;
	//链表节点个数加一
	list->size++;

	slist_node_s **p = &list->head;
	while (*p != NULL)
	{
		p = &(*p)->next;
	}
	*p = node_new;
	return 0;
}

//从链表中移除节点
int slist_remove(slist_s *list, slist_node_s **node, void **value)
{
	if (list == NULL)
	{
		return -1;
	}
	if (node == NULL)
	{
		return -1;
	}

	if (list->size == 0)
	{
		return -1;
	}

	//输出节点值
	if (value != NULL)
	{
		*value = (*node)->value;
	}

	//释放此节点
	slist_free(list, (*node)->ind);
	list->size--;
	//移除节点
	*node = (*node)->next;

	return 0;
}

//找到第一个空闲位置的索引号
uint32_t slist_first_empty(slist_s *list)
{
	if (list->use_map & 0xff)
	{
		return map_first_one[list->use_map & 0x00ff];
	}

	if (list->use_map & 0xff00)
	{
		return map_first_one[(list->use_map & 0xff00) >> 8] + 8;
	}

	if ((list->use_map & 0xff0000))
	{
		return map_first_one[(list->use_map & 0xff0000) >> 16] + 16;
	}

	if ((list->use_map & 0xff000000))
	{
		return map_first_one[(list->use_map & 0xff000000) >> 24] + 24;
	}
	return 0xffffffff;
}

//根据索引号分配节点
int slist_alloc(slist_s *list, uint32_t ind)
{
	if (list == NULL)
	{
		return -1;
	}

	//将使用位图中指定的索引号置为0,表示此位置占用
	list->use_map &= ~(1 << ind);

	return 0;
}

//根据索引号释放节点
int slist_free(slist_s *list, uint32_t ind)
{
	if (list == NULL)
	{
		return -1;
	}

	//将使用位图中指定的索引号置为1,表示此位置空闲
	list->use_map |= 1 << ind;

	return 0;
}
