/***
 * 
 * Ticket Locks
 * Figure 28.7 p.327
 * 
***/

#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <stdlib.h>

typedef struct __lock_t
{
    int ticket; 
    int turn; 
} lock_t;

/* This procedure have atomically operations */
/* A hardware primitive (an instruction) */
int fetch_and_add(int* ptr)
{
    int old = *ptr; 
    *ptr = old + 1; 
    return old; 
}

void lock_init(lock_t* lock)
{
    lock->ticket = 0;  
    lock->turn = 0; 
}

void lock(lock_t* lock)
{
    int myturn = fetch_and_add(&lock->ticket); 
    while (lock->turn != myturn); /* Spin */
}

void unlock(lock_t* lock)
{
    lock->turn = lock->turn + 1; 
}


int main(int argc, char* argv[])
{
    return 0; 
}