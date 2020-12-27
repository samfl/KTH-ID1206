/***
 * 
 * A Simple Spin Lock Using Test-and-set
 * Figure 28.3 p.322
 * 
***/

#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <stdlib.h>

typedef struct __lock_t
{
    int flag; 
} lock_t;

/* This procedure have atomically operations */
/* A hardware primitive (an instruction) */
int test_and_set(int* old_ptr, int new)
{
    int old = *old_ptr; /* fetch old val */
    *old_ptr = new;     /* store new into old_ptr */
    return old;         /* return old val */
}

void init(lock_t* lock)
{
    lock->flag = 0;  
}

void lock(lock_t* lock)
{
    while (Test_and_set(&lock->flag, 1) == 1); /* Spin-wait (do nothing) */
}

void unlock(lock_t* lock)
{
    lock->flag = 0; 
}


int main(int argc, char* argv[])
{
    return 0; 
}