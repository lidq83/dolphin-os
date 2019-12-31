/*
 * tree.h
 *
 *  Created on: December 31, 2019
 *      Author: lidq
 */
#ifndef __SRC_LIB_TREE_H
#define __SRC_LIB_TREE_H

typedef struct tree_s
{
    struct tree_s *child;
    struct tree_s *sibling;
    void *key;
    void *value;
} tree_s;

int tree_init();

#endif