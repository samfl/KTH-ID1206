/***
 * 
 * Lock With Test-and-set And Yield
 * Figure 28.8 p.328
 * 
***/

#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <stdlib.h>

static int volatile flag = 0; 

/* This procedure have atomically operations */
/* A hardware primitive (an instruction) */
int test_and_set(int* old_ptr, int new)
{
    int old = *old_ptr; /* fetch old val */
    *old_ptr = new;     /* store new into old_ptr */
    return old;         /* return old val */
}

void init(void)
{
    flag = 0; 
}

void lock(void)
{
    while (test_and_set(&flag, 1) == 1)
    {
        yield(); /* Give up the CPU. */
    }
}

void unlock(void)
{
    flag = 0; 
}

int main(int argc, char* argv[])
{
    return 0; 
}