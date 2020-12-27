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

int load_linked(int* ptr)
{
    return *ptr;
}

int store_cond(int *ptr, int value)
{
    if (1/* Noone has updated *ptr sinze the loadlinked to this address. */)
    {
        *ptr = value; 
        return 1; /* Success! */
    } else 
    {
        return 0; /* Failed to update */
    }
}

void lock(lock_t* lock)
{
    while (1) 
    {
        while (load_linked(&lock->flag) == 1); /* Spin until it's zero */
        if (store_cond(&lock->flag, 1) == 1) return; /* If set-it-to-1 as success: all done, otherwise: try it all over again */
    }
}

void unlock(lock_t* lock)
{
    lock->flag = 0; 
}

int main(int argc, char* argv[])
{
    return 0; 
}