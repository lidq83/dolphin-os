/*
 * fs.c
 *
 *  Created on: January 1, 2020
 *      Author: lidq
 */
#include <fs.h>

int fs_register_dev(char *path, file_operations_s ops)
{
    return vfs_insert_node(path, ops);
}

int fs_unregister_dev(char *path)
{
    return vfs_remove_node(path);
}

vfs_node_s *fs_get_node(char *path)
{
    return vfs_find_node(path);
}