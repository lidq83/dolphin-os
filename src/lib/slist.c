/*
 * slist.c
 *
 *  Created on: December 23, 2019
 *      Author: lidq
 */

#include "slist.h"

static uint8_t map_first_one[256] = {
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

int slist_init(slist_s *list)
{
	if (list == NULL)
	{
		return -1;
	}

	memset(list, 0, sizeof(slist_s));
	list->use_map = 0xffffffff;
	return 0;
}

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

	uint32_t ind = slist_first_empty(list);
	slist_alloc(list, ind);
	slist_node_s *node_new = &list->node_map[ind];
	node_new->ind = ind;
	node_new->key = key;
	node_new->value = value;
	node_new->next = NULL;

	list->size++;

	if (list->foot == NULL)
	{
		list->head = node_new;
		list->foot = node_new;
		return 0;
	}

	list->foot->next = node_new;
	list->foot = node_new;

	return 0;
}

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

	if (value != NULL)
	{
		*value = (*node)->value;
	}

	slist_free(list, (*node)->ind);
	*node = (*node)->next;

	return 0;
}

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

	return map_first_one[(list->use_map & 0xff000000) >> 24] + 24;
}

int slist_alloc(slist_s *list, uint32_t ind)
{
	if (list == NULL)
	{
		return -1;
	}
	list->use_map &= ~(1 << ind);

    return 0;
}

int slist_free(slist_s *list, uint32_t ind)
{
	if (list == NULL)
	{
		return -1;
	}
	list->use_map |= 1 << ind;

    return 0;
}
