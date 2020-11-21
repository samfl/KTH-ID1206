#include <stdlib.h>
#include <unistd.h>

void* malloc(size_t size)
{

    /* Input check */
    if (size == 0)
    {
        return NULL; 
    }

    /* Ask OS for memory! */
    /* With only sbrk, we'll run out of memory *. We must reuse the heap space. */
    void* memory = sbrk(size);

    /* On success, return value: 0. On failure, return value: -1. */
    if (memory == (void*) -1)
    {
        return NULL; 
    } else 
    {
        return memory; 
    }
}

/* Deallocation, not implemented yet. */
void free(void* memory)
{
    return; 
}

/*

In the first run it is the out-of-memory management process being invoked to kill the process. 
(kernel runs out of memory)

*/