#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define PAGES (4096)
#define REFS (1024*1024*10)

#define PAGESIZE (64)

int main(int argc, char *argv)
{   
    char* memory = malloc( (long) PAGESIZE * PAGES);

    for(int p = 0; p < PAGES; p++)
    {
        long ref = (long) p * PAGESIZE;
        /* Forces the page to be allocated */
        memory[ref]++;
    }
    printf("#TLB experiment\n");
    printf("#   page size = %d bytes\n", PAGESIZE);
    printf("#   max pages = %d\n", PAGES);
    printf("#   total number or references = %d Mi\n", REFS / (1024 / 1024));

    clock_t c_start;
    clock_t c_stop;

    printf("#Pages\t proc\t sum\n");

        for(int pages = 4; pages <= PAGES; pages*=2)
        {
            int loops = REFS / pages; 
            c_start = clock(); 

            long sum = 0;

            for(int l = 0; l < loops; l++)
            {
                for(int p = 0; p < pages; p++)
                {
                    /* Real Operations */
                    long ref = (long)p * PAGESIZE;
                    sum += memory[ref];
                    //sum++; 
                }
            }

            c_stop = clock(); 

            {
                double proc; 
                proc = ((double) (c_stop - c_start)) / CLOCKS_PER_SEC;
                printf("%d\t %.6f\t %ld \n", pages, proc, sum);
            }
        }
        return 0; 
}