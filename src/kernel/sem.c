/*
 * sem.c
 *
 *  Created on: December 23, 2019
 *      Author: lidq
 */
#include <sem.h>
#include <sche.h>

int sem_init(sem_t *sem, int value)
{
    if (sem == NULL)
    {
        return -1;
    }
    slist_init(&sem->wait_queue);
    sem->value = value;

    return 0;
}

int sem_wait(sem_t *sem)
{
    if (sem == NULL)
    {
        return -1;
    }

    sche_int_enter();

    sem->value--;
    if (sem->value < 0)
    {
        pcb_s *p_curr = sche_curr_pcb();
        pcb_block(p_curr);
        slist_append(&sem->wait_queue, p_curr, p_curr);
        sche_switch();
    }

    sche_int_leave();

    return 0;
}

int sem_post(sem_t *sem)
{
    if (sem == NULL)
    {
        return -1;
    }

    sche_int_enter();

    if (sem->value < 0)
    {
        slist_node_s **p = &(sem->wait_queue.head);
        if (*p != NULL)
        {
            *p = (*p)->next;
            pcb_s *pcb = (pcb_s *)(*p)->value;
            pcb_ready(pcb);
            sche_switch();
        }
    }
    sem->value++;

    sche_int_leave();

    return 0;
}