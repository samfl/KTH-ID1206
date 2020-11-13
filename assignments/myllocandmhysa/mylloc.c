#include <stdlib.h>
#include <unistd.h>

void* malloc(size_t size)
{
    if (size == 0)
    {
        return NULL; 
    }

    void* memory = sbrk(size);

    if (memory == (void* ) 1 )
    {
        return NULL; 
    } else 
    {
        return memory; 
    }
}

void free(void* memory)
{
    return; 
}

/*

In the first run it is the out-of-memory management pocess being invoked to kill the process. 
(kernel runs out of memory)

*/