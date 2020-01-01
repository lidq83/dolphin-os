/*
 * vfs.h
 *
 *  Created on: December 31, 2019
 *      Author: lidq
 */
#ifndef __SRC_VFS_VFS_H
#define __SRC_VFS_VFS_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <sem.h>

#define NODE_NAME_SIZE (64)

struct file
{
};

typedef struct file_operations_s
{
	int (*open)(struct file *);
	int (*close)(struct file *);
	size_t (*read)(struct file *, void *, size_t);
	size_t (*write)(struct file *, const void *, size_t);
	int (*ioctl)(struct file *, unsigned int, unsigned long);
} file_operations_s;

typedef struct vfs_node_s
{
	struct vfs_node_s *sibling;
	struct vfs_node_s *child;
	char name[NODE_NAME_SIZE];
	struct file_operations_s ops;
} vfs_node_s;

typedef struct vfs_s
{
	vfs_node_s *root;
	sem_t sem_rw;
} vfs_s;

int vfs_init(void);

int vfs_destory(void);

vfs_node_s *vfs_find_node(char *path);

int vfs_insert_node(char *path, file_operations_s ops);

int vfs_remove_node(char *path);

#endif
