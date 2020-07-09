/*
 * slist.h
 *
 *  Created on: December 23, 2019
 *      Author: lidq
 */
#ifndef __SRC_LIB_SLIST_H
#define __SRC_LIB_SLIST_H

#include <stdint.h>
#include <stdio.h>
#include <string.h>

//静态链表最大个数32
#define SLIST_SIZE (32)

//链表节点结构体
typedef struct slist_node_s
{
	//下一个结节
	struct slist_node_s *next;
	//关键字
	void *key;
	//值
	void *value;
} slist_node_s;

//链表结构体
typedef struct slist_s
{
	//链表头节点
	slist_node_s *head;
} slist_s;

//初始化链表
int slist_init(slist_s *list);

//追加节点到链表
int slist_append(slist_s *list, void *key, void *value);

//从链表中移除节点
int slist_remove(slist_s *list, slist_node_s **node, void **value);

#endif