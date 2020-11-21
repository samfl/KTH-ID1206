#include <stdlib.h>
#include <unistd.h>

/* Data structure for holding a free block */
/* We allocate more space than requested but save the size in the beginning of the block */
struct chunk
{
    int size; 
    struct chunk* next; 
};

/* Linked list structure that will hold the freed blocks */
struct chunk* flist = NULL; 

/* Re-written malloc to use the freed list */
void* malloc(size_t size)
{   
    /* Input check */
    if (size == 0)
    {
        return NULL; 
    }


    struct chunk* next = flist; 
    struct chunk* prev = NULL; 

    while(next != NULL)
    {
        if (next->size >= size)
        {
            if (prev != NULL)
            {
                prev->next = next->next; 
            } else
            {
                flist = next->next; 
            }
            return (void*)(next + 1); 
        } else 
        {
            prev = next; 
            next = next->next; 
        }
    }

    /* use sbrk to allocate new memory */
    void* memory = sbrk(size + sizeof(struct chunk));
    if (memory == (void* ) -1 )
    {
        return NULL; 
    } else 
    {
        struct chunk *cnk = (struct chunk*) memory; 
        cnk->size = size; 
        return (void*)(cnk + 1); 
    }
}

/* Re-written free to use the freed list */
void free(void* memory)
{
    if (memory != NULL)
    {   
        /* Copy the chunk hidden in memory */
        /* Add it first to the list */
        /* Add the flist "head" to the first chunk */
        struct chunk* cnk = (struct chunk*)((struct chunk*)memory - 1); 
        cnk->next = flist; 
        flist = cnk; 
    }
    return; 
}

/*

In the first run it is the out-of-memory management pocess being invoked to kill the process. 
(kernel runs out of memory)

*/