#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "rand.h"
#include "firstfit.h"

/* Outer-loop bound */
#define ROUNDS 1000

/* Inner-loop bound */
#define LOOPS 100000

/* Buffer-bound (buffer used to simulate a regular program) */
#define BUFFER 100

int main(void)
{   
    /* Creating the buffer */
    void* buffer[BUFFER];
    for(int i = 0; i < BUFFER; i++)
    {
        buffer[i] = NULL; 
    }

    /* Find current location of the program break (which defines the end of the processes's data segment) */
    void* init = sbrk(0);
    void* current; 

    printf("The initial top of the heap is %p.\n", init);

    for(int j = 0; j < ROUNDS; j++)
    {
        for(int i = 0; i < LOOPS; i++)
        {
            /* Calculate the index */
            int index = rand() % BUFFER; 

            /* If the randomized buffer index contains something, we want to free it. */
            if(buffer[index] != NULL)
            {   
                /* Free the memory */
                bree(buffer[index]);

                /* Reset the memory */
                buffer[index] = NULL; 
            } else 
            {
                /* Request a size (exponential decrease distribution, more smaller chunks are normal in a program), */
                size_t size = (size_t) request(); 

                /* Allocate the memory based on the requested size */
                int* memory; 
                memory = balloc(size); 

                /* Check if malloc was OK. */
                if (memory == NULL)
                {
                    fprintf(stderr, "malloc failed\n");
                    return(1); 
                }

                /* Assign the pointer to the allocated memory to our buffer. */
                buffer[index] = memory; 

                /* writing to the memory so know it exists */
                *memory = 123; 
            }
        }

        /* Again, check current location of the program break (which defines the end of the processes's data segment) */
        current = sbrk(0);

        /* Calculate the size of the allocation */
        int allocated = (int)((current - init ) / 1024);

        /* Print info! */
        printf("%d\n", j);
        printf("The current top of the heap is %p. \n", current);
        printf("increased by %d Kbyte\n", allocated);
    }
    return 0; 
}

/* 
Compile and link bench with mylloc: 
> gcc -o bench mylloc.o bench.c 
*/

