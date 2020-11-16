#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/* #define ROUNDS 10 - OK, successful */
/* #define ROUNDS 100 - Killed */
#define ROUNDS 1000
#define LOOPS 100000

int main(void)
{
    void* init = sbrk(0);
    void* current; 

    printf("The initial top of the heap is %p.\n", init);

    for(int j = 0; j < ROUNDS; j++)
    {
        for(int i = 0; i < LOOPS; i++)
        {
            size_t size = (rand() % 4000) + sizeof(int);
            int* memory; 
            memory = malloc(size);

            if (memory == NULL)
            {
                fprintf(stderr, "malloc failed\n");
                return(1); 
            }
            /* writing to the memory so we know it exists */
            /* *memory = 123; - Commented Out, ROUNDS changed to 1000. */
            free(memory);
        }
        current = sbrk(0);
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

