#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/* #define ROUNDS 10 - OK, successful */
/* #define ROUNDS 100 - Killed */
#define ROUNDS 1000
#define LOOPS 100000

int main(void)
{   
    /* Find current location of the program break (which defines the end of the processes's data segment). */
    void* init = sbrk(0);
    void* current; 

    printf("The initial top of the heap is %p.\n", init);

    /* Outer-loop (ROUNDS) */
    for(int j = 0; j < ROUNDS; j++)
    {   
        /* Inner-loop (LOOPS) */
        for(int i = 0; i < LOOPS; i++)
        {
            /* Randomize the size of the block (in bytes [8 bits]) to be allocated */
            /* Uniform distribution of size from 0 - 4000 */
            size_t size = (rand() % 4000) + sizeof(int);

            /* Allocate the memory */
            int* memory; 
            memory = malloc(size);

            /* Check if malloc was successful */
            if (memory == NULL)
            {
                fprintf(stderr, "malloc failed\n");
                return(1); 
            }

            /* writing to the memory so we know it exists */
            //*memory = 123;

            /* Free the memory (deallocation) */
            free(memory);
        }

        /* Find current location of the program break (which defines the end of the processes's data segment). */
        current = sbrk(0);

        /* Calculate the size of the allocation */
        int allocated = (int)((current - init ) / 1024);
        printf("%d\n", j);
        printf("The current top of the heap is %p. \n", current);
        printf("Increased by %d Kbyte\n", allocated);
    }
    return 0; 
}

/* 
Compile and link bench with mylloc: 
> gcc -o bench mylloc.o bench.c 
*/

