/*
 * fcntl.h
 *
 *  Created on: January 1, 2020
 *      Author: lidq
 */
#ifndef __SRC_VFS_FCNTL_H
#define __SRC_VFS_FCNTL_H

#include <sys/types.h>
#include <fs.h>
#include <k_printf.h>

int open(char *path, int oflag, int mode);

int close(int fd);

size_t read(int fd, void *buf, size_t count);

size_t write(int fd, void *buf, size_t count);

int ioctl(int fd, unsigned int cmd, unsigned long arg);

#endif
