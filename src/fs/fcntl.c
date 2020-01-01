/*
 * fcntl.c
 *
 *  Created on: January 1, 2020
 *      Author: lidq
 */
#include <fcntl.h>
#include <sche.h>
#include <fs.h>

const uint8_t map_fuse[256] = {
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

//找到第一个空闲位置的索引号
static uint32_t fcntl_first_empty(pcb_s *pcb);

//根据索引号分配节点
static int fcntl_alloc(pcb_s *pcb, uint32_t ind);

//根据索引号释放节点
static int fcntl_free(pcb_s *pcb, uint32_t ind);

int open(char *path, int oflag, int mode)
{
    vfs_node_s *node = fs_get_node(path);
    if (node == NULL)
    {
        return -1;
    }

    pcb_s *pcb = sche_curr_pcb();

    if (node->ops.open == NULL)
    {
        return -1;
    }
    node->ops.open(NULL);

    uint32_t ind = fcntl_first_empty(pcb);
    if (ind >= FNODE_SIZE)
    {
        return -1;
    }

    //申请节点
    fcntl_alloc(pcb, ind);
    pcb->fnodes[ind] = node;

    return ind;
}

int close(int fd)
{
    if (fd < 0 || fd >= FNODE_SIZE)
    {
        return -1;
    }

    pcb_s *pcb = sche_curr_pcb();
    vfs_node_s *node = pcb->fnodes[fd];
    if (node == NULL)
    {
        return -1;
    }

    if (node->ops.close == NULL)
    {
        return -1;
    }

    int ret = node->ops.close(NULL);

    pcb->fnodes[fd] = NULL;
    fcntl_free(pcb, fd);

    return ret;
}

size_t read(int fd, void *buf, size_t count)
{
    if (fd < 0 || fd >= FNODE_SIZE)
    {
        return -1;
    }

    pcb_s *pcb = sche_curr_pcb();
    vfs_node_s *node = pcb->fnodes[fd];
    if (node == NULL)
    {
        return -1;
    }

    if (node->ops.read == NULL)
    {
        return -1;
    }

    return node->ops.read(NULL, buf, count);
}

size_t write(int fd, void *buf, size_t count)
{
    if (fd < 0 || fd >= FNODE_SIZE)
    {
        return -1;
    }

    pcb_s *pcb = sche_curr_pcb();
    vfs_node_s *node = pcb->fnodes[fd];
    if (node == NULL)
    {
        return -1;
    }

    if (node->ops.write == NULL)
    {
        return -1;
    }

    return node->ops.write(NULL, buf, count);
}

int ioctl(int fd, unsigned int cmd, unsigned long arg)
{
    if (fd < 0 || fd >= FNODE_SIZE)
    {
        return -1;
    }

    pcb_s *pcb = sche_curr_pcb();
    vfs_node_s *node = pcb->fnodes[fd];
    if (node == NULL)
    {
        return -1;
    }

    if (node->ops.ioctl == NULL)
    {
        return -1;
    }

    return node->ops.ioctl(NULL, cmd, arg);
}

//找到第一个空闲位置的索引号
uint32_t fcntl_first_empty(pcb_s *pcb)
{
    if (pcb->f_use_map & 0xff)
    {
        return map_fuse[pcb->f_use_map & 0x00ff];
    }

    if (pcb->f_use_map & 0xff00)
    {
        return map_fuse[(pcb->f_use_map & 0xff00) >> 8] + 8;
    }

    if ((pcb->f_use_map & 0xff0000))
    {
        return map_fuse[(pcb->f_use_map & 0xff0000) >> 16] + 16;
    }

    if ((pcb->f_use_map & 0xff000000))
    {
        return map_fuse[(pcb->f_use_map & 0xff000000) >> 24] + 24;
    }
    return 0xffffffff;
}

//根据索引号分配节点
int fcntl_alloc(pcb_s *pcb, uint32_t ind)
{
    if (pcb == NULL)
    {
        return -1;
    }

    //将使用位图中指定的索引号置为0,表示此位置占用
    pcb->f_use_map &= ~(1 << ind);

    return 0;
}

//根据索引号释放节点
int fcntl_free(pcb_s *pcb, uint32_t ind)
{
    if (pcb == NULL)
    {
        return -1;
    }

    //将使用位图中指定的索引号置为1,表示此位置空闲
    pcb->f_use_map |= 1 << ind;

    return 0;
}