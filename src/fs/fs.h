/*
 * fs.h
 *
 *  Created on: January 1, 2020
 *      Author: lidq
 */
#ifndef __SRC_VFS_FS_H
#define __SRC_VFS_FS_H

#include <vfs.h>
#include <k_printf.h>

int fs_register_dev(char *path, file_operations_s ops);

int fs_unregister_dev(char *path);

vfs_node_s *fs_get_node(char *path);

#endif