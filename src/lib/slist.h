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
#include <k_printf.h>
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
	//索引号
	uint32_t ind;
} slist_node_s;

//链表结构体
typedef struct slist_s
{
	//链表头节点
	slist_node_s *head;
	//链表节点使用位图（0：使用，1：空闲）
	uint32_t use_map;
	//链表中节点个数
	uint8_t size;
	//链表静态分配节点
	slist_node_s node_map[SLIST_SIZE];
} slist_s;

//初始化链表
int slist_init(slist_s *list);

//追加节点到链表
int slist_append(slist_s *list, void *key, void *value);

//从链表中移除节点
int slist_remove(slist_s *list, slist_node_s **node, void **value);

//找到第一个空闲位置的索引号
uint32_t slist_first_empty(slist_s *list);

//根据索引号分配节点
int slist_alloc(slist_s *list, uint32_t ind);

//根据索引号释放节点
int slist_free(slist_s *list, uint32_t ind);

#endif