#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "rand.h"
#include "dlmall.h"

#define ROUNDS 10
#define LOOPS 1000
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
        for(int i = 0; i < LOOPS; i++)
        {
        	printf("Loop    -> %d \n", i);
            int index = rand() % BUFFER;

            if(buffer[index] != NULL)
            {
            	printf("Calling  -> Dree\n");
                dree(buffer[index]);
                buffer[index] = NULL;
                ++num_of_drees;
            } else
            {
                size_t size = (size_t) request();
                printf("Calling -> Dalloc (%ld bytes) \n", size);

                int* memory;
                memory = dalloc(size);

                if (memory == NULL)
                {
                    fprintf(stderr, "malloc failed\n");
                    return(1);
                }
                ++num_of_dallocs;

                buffer[index] = memory;
                *memory = 123;
            }
            int prog_status = sanity();
            if (prog_status)
            {
            	switch(prog_status)
            	{
            		case 1:
            			printf("Incorrect free-status\n");
					break;
                	case 2:
                		printf("Incorrect size\n");
					break;
                   	case 3:
                   		printf("Incorrect size of adjacent block \n");
					break;
                   	case 4:
                   		printf("Incorrect free of adjacent blocks \n");
					break;
                   	case 5:
                   		printf("Incorrect arena \n");
					break;
                   	case 6:
                   		printf("Incorrect free in adjacent arena blocks \n");
					break;
                   	case 7:
                   		printf("Incorrect size in adjacent arena blocks \n");
					break;
                   	case 8:
                   		printf("Incorrect alignment \n");
					break;
                   	case 9:
                   		printf("Incorrect size2 \n");
					break;
                   	default:

                   	break;
            	}
            	return 1;
            }
            flist_info();
        }
        current = sbrk(0);
        int allocated = (int)((current - init ) / 1024);

        printf("The current top of the heap is %p. \n", current);
        printf("increased by %d Kbyte\n", allocated);
        printf("Total num of dree's: %d\n", num_of_drees);
        printf("Total num of dalloc's: %d\n", num_of_dallocs);
    }
    return 0;
}
