/*
 * sem.h
 *
 *  Created on: December 20, 2019
 *      Author: lidq
 */
#ifndef __SRC_KERNEL_SEM_H
#define __SRC_KERNEL_SEM_H

#include <slist.h>

typedef struct sem_t
{
    int value;
    slist_s wait_queue;
} sem_t;

int sem_init(sem_t *sem, int value);

int sem_wait(sem_t *sem);

int sem_post(sem_t *sem);

#endif