/*
 * sem.h
 *
 *  Created on: December 20, 2019
 *      Author: lidq
 */
#ifndef __SRC_KERNEL_SEM_H
#define __SRC_KERNEL_SEM_H

#include <slist.h>

//信号量结构体
typedef struct sem_t
{
    //信号量的值，大于0表示资源个数，小于0表示等待资源的进程个数
    int value;
    //等待队列
    slist_s wait_queue;
} sem_t;

//初始化信号量
int sem_init(sem_t *sem, int value);

//等待信号量
int sem_wait(sem_t *sem);

//释放信号量
int sem_post(sem_t *sem);

#endif