/*
 * slist.h
 *
 *  Created on: December 23, 2019
 *      Author: lidq
 */
#ifndef __SRC_LIB_SLIST_H
#define __SRC_LIB_SLIST_H

#include <stdint.h>
#include <string.h>

#define SLIST_SIZE (32)

typedef struct slist_node_s
{
	struct slist_node_s *next;
	void *key;
	void *value;
	uint32_t ind;
} slist_node_s;

typedef struct slist_s
{
	slist_node_s *head;
	slist_node_s *foot;
	uint32_t size;
	uint32_t use_map;
	slist_node_s node_map[SLIST_SIZE];
} slist_s;

int slist_init(slist_s *list);

int slist_append(slist_s *list, void *key, void *value);

int slist_remove(slist_s *list, slist_node_s **node, void **value);

uint32_t slist_first_empty(slist_s *list);

int slist_alloc(slist_s *list, uint32_t ind);

int slist_free(slist_s *list, uint32_t ind);

#endif