/*
 * slist.c
 *
 *  Created on: December 23, 2019
 *      Author: lidq
 */

#include <slist.h>
#include <mm.h>

//初始化链表
int slist_init(slist_s *list)
{
	if (list == NULL)
	{
		return -1;
	}

	memset(list, 0, sizeof(slist_s));
	return 0;
}

//追加节点到链表
int slist_append(slist_s *list, void *key, void *value)
{
	if (list == NULL)
	{
		return -1;
	}

	//设置新节点的值
	slist_node_s *node_new = malloc(sizeof(slist_node_s));
	if (node_new == NULL)
	{
		return -1;
	}
	//关键字
	node_new->key = key;
	//值
	node_new->value = value;
	//下一个节点
	node_new->next = NULL;

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
	if (*node == NULL)
	{
		return -1;
	}

	//输出节点值
	if (value != NULL)
	{
		*value = (*node)->value;
	}

	slist_node_s *del = *node;
	//移除节点
	*node = (*node)->next;
	free(del);

	return 0;
}
