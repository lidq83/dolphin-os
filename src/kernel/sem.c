/*
 * sem.c
 *
 *  Created on: December 23, 2019
 *      Author: lidq
 */
#include <sem.h>
#include <sche.h>

extern pcb_s *pcb_current;
extern pcb_s *pcb_next;

extern void sche_interrupt_disable(void);
extern void sche_interrupt_enable(void);

//初始化信号量
int sem_init(sem_t *sem, int value)
{
    if (sem == NULL)
    {
        return -1;
    }

    //初始化信号量等待队列
    slist_init(&sem->wait_queue);
    //初始化资源个数
    sem->value = value;

    return 0;
}

//等待信号量
int sem_wait(sem_t *sem)
{
    if (sem == NULL)
    {
        return -1;
    }

    //关中断
    sche_interrupt_disable();

    //信号量的值减一
    sem->value--;
    //如果信号量的值小于0,则当前进程需要等待
    if (sem->value < 0)
    {
        //取得当前运行的进程
        pcb_s *p_curr = sche_curr_pcb();
        //挂起此进程
        pcb_block(p_curr);
        //将此进程加入到信号量的等待队列
        slist_append(&sem->wait_queue, p_curr, p_curr);
        pcb_next = pcb_get_highest_pcb();
        //如果最高优先级进程就是当前进程，则进行跳转返回
        if (pcb_current != pcb_next)
        {
            //触发调度
            sche_switch();
        }
    }

    //开中断
    sche_interrupt_enable();

    return 0;
}

//释放信号量
int sem_post(sem_t *sem)
{
    if (sem == NULL)
    {
        return -1;
    }

    //关中断
    sche_interrupt_disable();

    //如果信号量小于0,表示需要从等待队列中唤醒一个进程
    if (sem->value < 0)
    {
        //从等待队列中取出头节点，即队列头
        slist_node_s **p = &(sem->wait_queue.head);
        if (*p != NULL)
        {
            //取得头节点中的进程pcb
            pcb_s *pcb = (pcb_s *)(*p)->value;
            //将队列头节点移出队列
            slist_remove(&sem->wait_queue, p, NULL);
            //将此进程加入就绪队列
            pcb_ready(pcb);

            pcb_next = pcb_get_highest_pcb();
            //如果最高优先级进程就是当前进程，则进行跳转返回
            if (pcb_current != pcb_next)
            {
                //触发调度
                sche_switch();
            }
        }
    }
    //资源数加一
    sem->value++;

    //开中断
    sche_interrupt_enable();

    return 0;
}