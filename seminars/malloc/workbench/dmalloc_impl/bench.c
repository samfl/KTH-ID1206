#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "rand.h"
#include "dlmall.h"

/* Outer-loop bound */
#define ROUNDS 10

/* Inner-loop bound */
#define LOOPS 1000

/* Buffer-bound (buffer used to simulate a regular program) */
#define BUFFER 100

int main(void)
{
	int num_of_dallocs = 0;
	int num_of_drees = 0;

    void* buffer[BUFFER];
    for(int i = 0; i < BUFFER; i++)
    {
        buffer[i] = NULL;
    }

    void* init = sbrk(0);
    void* current;

    printf("The initial top of the heap is %p.\n", init);

    for(int j = 0; j < ROUNDS; j++)
    {
        printf("Outer-loop val (j): %d\n\t", j);
        for(int i = 0; i < LOOPS; i++)
        {
            printf("Inner-loop val (i): %d\n\t", i);

            int index = rand() % BUFFER;
            printf("\t buffer[%d]: %p, ", index, buffer[index]);

            if(buffer[index] != NULL)
            {
                dree(buffer[index]);
                buffer[index] = NULL;
                printf("\t buffer[%d]: %p, freed\n\t", index, buffer[index]);
                ++num_of_drees;
            } else
            {
                size_t size = (size_t) request();
                printf("\t size: %ld, ", size);

                int* memory;
                memory = dalloc(size);

                if (memory == NULL)
                {
                    fprintf(stderr, "malloc failed\n");
                    printf("length of list: %d \t len_flist: %d \t size of block: %d \n", size, len_of_flist(), blocksize_of_flist());
                    return(1);
                }
                ++num_of_dallocs;

                printf("\t Setting: buffer[%d] to %d at %p, ", index, memory, memory);
                buffer[index] = memory;
                printf("\t Result: buffer[%d]: %p", index, buffer[index]);
                *memory = 123;

                void* curr = sbrk(0);
                int alloc = (int)((curr - init ) / 1024);
                printf("\t inc by %d Kb\n\t", alloc);
            }
        }
        current = sbrk(0);
        int allocated = (int)((current - init ) / 1024);

        /* Print info! */
        printf("%d\n", j);
        printf("The current top of the heap is %p. \n", current);
        printf("increased by %d Kbyte\n", allocated);

        printf("Total num of dree's: %d\n", num_of_drees);
        printf("Total num of dalloc's: %d\n", num_of_dallocs);
    }
    return 0;
}

/*
Compile and link bench with mylloc:
> gcc -o bench mylloc.o bench.c
*/

