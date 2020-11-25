#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "rand.h"
#include "dlmall.h"

#define ROUNDS 10
#define LOOPS 10000000
#define BUFFER 100

int main(void)
{

    void* buffer[BUFFER];
    for(int i = 0; i < BUFFER; i++)
    {
        buffer[i] = NULL;
    }

    void* init = sbrk(0);
    void* current;

    printf("The initial top of the heap is %p.\n", init);

    double time_spent_tot;
    double time_spent_round;
    clock_t begin;
    clock_t end;

    for(int j = 0; j < ROUNDS; j++)
    {
    	begin = clock();
        for(int i = 0; i < LOOPS; i++)
        {
            int index = rand() % BUFFER;

            if(buffer[index] != NULL)
            {
                dree(buffer[index]);
                buffer[index] = NULL;
            } else
            {
                size_t size = (size_t) request();

                int* memory;
                memory = dalloc(size);

                if (memory == NULL)
                {
                    fprintf(stderr, "malloc failed\n");
                    return(1);
                }

                buffer[index] = memory;
                *memory = 123;
            }
        }
        current = sbrk(0);
        int allocated = (int)((current - init ) / 1024);

        printf("The current top of the heap is %p. \n", current);
        printf("increased by %d Kbyte\n", allocated);

        end = clock();
        time_spent_round = (double)(end - begin) / CLOCKS_PER_SEC;
        time_spent_tot += time_spent_round;
    }
s
    printf("Time spent average: %f \n ", time_spent_tot/ROUNDS);
    flist_info();
    return 0;
}
