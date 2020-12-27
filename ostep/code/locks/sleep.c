/***
 * 
 * Lock With Queues, Test-and-set, Yield, And Wakeup
 * Figure 28.9 p.330
 * 
***/

#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/queue.h>

typedef struct __lock_t
{
    int flag; 
    int guard; 
    queue_t* q;
} lock_t; 

void lock_init(lock_t* m)
{
    m->flag = 0; 
    m->guard = 0; 
    queue_init(m->q); 
}

void lock(lock_t* m)
{
    while (test_and_set(&m->guard, 1) == 1); /* Acquire guard lock by spinning */
    if (m->flag == 0)
    {
        m->flag = 1; /* Lock is acquired */
        m->guard = 0; 
    } else 
    {
        queue_add(m->q, getid());
        m->guard = 0; 
        park(); 
    }
}

void unlock(lock_t* m)
{
    while (test_and_set(&m->guard, 1) == 1); /* Acquire guard lock by spinning */
    if (queue_empty(m->q))
    {
        m->flag = 0; /* Let go of lock, no one wants it */
    } else 
    {
        unpark(queue_remove(m->q)); /* Hold lock (for next thread) */
    }
    m->guard = 0; 
}

int main(int argc, char* argv[])
{
    return 0; 
}