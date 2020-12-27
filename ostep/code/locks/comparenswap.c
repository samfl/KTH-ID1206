/***
 * 
 * A Simple Spin Lock Using Compare-and-swap
 * Figure 28.4 p.324
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
int compare_and_swap(int* ptr, int expected, int new)
{
    int actual = *ptr; 
    if (actual == expected)
    {
        *ptr = new; 
    }
    return actual; 
}

void init(lock_t* lock)
{
    lock->flag = 0;  
}

void lock(lock_t* lock)
{
    while (compare_and_swap(&lock->flag, 0, 1) == 1); /* Spin */
}

void unlock(lock_t* lock)
{
    lock->flag = 0; 
}


int main(int argc, char* argv[])
{
    return 0; 
}