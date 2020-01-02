/*
 * vfs.c
 *
 *  Created on: December 31, 2019
 *      Author: lidq
 */
#include "vfs.h"

static vfs_s *vfs = NULL;

static void vfs_destory_r(vfs_node_s **node);

static vfs_node_s *vfs_find_node_r(vfs_node_s *node, char *abs_path);

static vfs_node_s *vfs_find_node_in_sibling(vfs_node_s *node, char *node_name);

static int vfs_insert_node_r(vfs_node_s **node, char *abs_path, file_operations_s ops);

static int vfs_remove_node_r(vfs_node_s **node, char *abs_path);

int vfs_init(void)
{
    vfs = malloc(sizeof(vfs_s));
    sem_init(&vfs->sem_rw, 1);
    vfs->root = malloc(sizeof(vfs_node_s));
    k_strcpy(vfs->root->name, "/");
    vfs->root->child = NULL;
    vfs->root->sibling = NULL;

    vfs->root->ops.open = NULL;
    vfs->root->ops.close = NULL;
    vfs->root->ops.read = NULL;
    vfs->root->ops.write = NULL;
    vfs->root->ops.ioctl = NULL;

    return 0;
}

int vfs_destory(void)
{
    sem_wait(&vfs->sem_rw);
    vfs_destory_r(&vfs->root);
    sem_post(&vfs->sem_rw);
    free(vfs);
    return 0;
}

vfs_node_s *vfs_find_node(char *path)
{
    if (path[0] != '/')
    {
        return NULL;
    }

    if (path[1] == 0)
    {
        return vfs->root;
    }

    sem_wait(&vfs->sem_rw);
    vfs_node_s *node = vfs_find_node_r(vfs->root->child, &path[1]);
    sem_post(&vfs->sem_rw);

    return node;
}

int vfs_insert_node(char *path, file_operations_s ops)
{
    if (path[0] != '/')
    {
        return -1;
    }
    sem_wait(&vfs->sem_rw);
    int ret = vfs_insert_node_r(&vfs->root->child, &path[1], ops);
    sem_post(&vfs->sem_rw);
    return ret;
}

int vfs_remove_node(char *path)
{
    if (path[0] != '/')
    {
        return 0;
    }

    if (path[1] == 0)
    {
        return -1;
    }

    sem_wait(&vfs->sem_rw);
    int ret = vfs_remove_node_r(&vfs->root->child, &path[1]);
    sem_post(&vfs->sem_rw);

    return ret;
}

void vfs_destory_r(vfs_node_s **node)
{
    if (node == NULL)
    {
        return;
    }

    if (*node == NULL)
    {
        return;
    }

    vfs_destory_r(&(*node)->child);
    vfs_destory_r(&(*node)->sibling);

    (*node)->child = NULL;
    (*node)->sibling = NULL;
    free(*node);
    *node = NULL;
}

vfs_node_s *vfs_find_node_r(vfs_node_s *node, char *abs_path)
{
    if (node == NULL)
    {
        return NULL;
    }
    if (abs_path == NULL)
    {
        return NULL;
    }
    if (abs_path[0] == 0)
    {
        return NULL;
    }

    char node_name[NODE_NAME_SIZE] = {0};

    char *p = abs_path;
    for (int i = 0; *p != '/' && *p != 0 && i < NODE_NAME_SIZE; i++)
    {
        node_name[i] = *p++;
    }

    vfs_node_s *ret = vfs_find_node_in_sibling(node, node_name);
    if (*p == '/')
    {
        p++;
        return vfs_find_node_r(ret->child, p);
    }
    return ret;
}

vfs_node_s *vfs_find_node_in_sibling(vfs_node_s *node, char *node_name)
{
    while (node != NULL)
    {
        if (k_strcmp(node->name, node_name) == 0)
        {
            return node;
        }
        node = node->sibling;
    }
    return NULL;
}

int vfs_insert_node_r(vfs_node_s **node, char *abs_path, file_operations_s ops)
{
    if (node == NULL)
    {
        return -1;
    }
    if (abs_path == NULL)
    {
        return -1;
    }
    if (abs_path[0] == 0)
    {
        return -1;
    }

    char node_name[NODE_NAME_SIZE] = {0};

    char *p = abs_path;
    for (int i = 0; *p != '/' && *p != 0 && i < NODE_NAME_SIZE; i++)
    {
        node_name[i] = *p++;
    }

    if (*p == '/' && *p != 0)
    {
        p++;
    }

    while (*node != NULL)
    {
        if (k_strcmp((*node)->name, node_name) == 0)
        {
            return vfs_insert_node_r(&(*node)->child, p, ops);
        }
        node = &(*node)->sibling;
    }

    vfs_node_s *node_new = malloc(sizeof(vfs_node_s));
    k_strcpy(node_new->name, node_name);

    node_new->child = NULL;
    node_new->sibling = NULL;
    k_memset(&node_new->ops, 0, sizeof(file_operations_s));

    *node = node_new;

    if (*p != 0)
    {
        return vfs_insert_node_r(&node_new->child, p, ops);
    }

    k_memcpy(&node_new->ops, &ops, sizeof(file_operations_s));
    return 0;
}

int vfs_remove_node_r(vfs_node_s **node, char *abs_path)
{
    if (node == NULL)
    {
        return -1;
    }
    if (abs_path == NULL)
    {
        return -1;
    }
    if (abs_path[0] == 0)
    {
        return -1;
    }

    char node_name[NODE_NAME_SIZE] = {0};

    char *p = abs_path;
    for (int i = 0; *p != '/' && *p != 0 && i < NODE_NAME_SIZE; i++)
    {
        node_name[i] = *p++;
    }

    while (*node != NULL)
    {
        if (k_strcmp((*node)->name, node_name) == 0)
        {
            goto _found_node;
        }
        node = &(*node)->sibling;
    }

    return -1;

_found_node:;

    if (*p == '/')
    {
        p++;
        return vfs_remove_node_r(&(*node)->child, p);
    }

    if ((*node)->child != NULL)
    {
        return -1;
    }

    vfs_node_s *node_del = *node;
    *node = (*node)->sibling;
    free(node_del);

    return 0;
}
