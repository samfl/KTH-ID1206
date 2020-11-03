#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{   

    int pid = getpid();

    long* heap = (unsigned long*) calloc(80, sizeof(unsigned long));

    /* 'lx' in the formatted print expects the type long unsigned int */
    printf("heap[2]: 0x%lx\n", heap[2]);
    printf("heap[1]: 0x%lx\n", heap[1]);
    printf("heap[0]: 0x%lx\n", heap[0]);
    printf("heap[-1]: 0x%lx\n", heap[-1]);
    printf("heap[-2]: 0x%lx\n", heap[-2]);

    /* Additional code added */
    free(heap);
    printf("\n After free: \n");
    
    printf("heap[2]: 0x%lx\n", heap[2]);
    printf("heap[1]: 0x%lx\n", heap[1]);
    printf("heap[0]: 0x%lx\n", heap[0]);
    printf("heap[-1]: 0x%lx\n", heap[-1]);
    printf("heap[-2]: 0x%lx\n", heap[-2]);
    /* Memory trace */
    back: 
        printf(" back: %p \n", &&back);
        printf(" \n\n /proc/%d/maps \n\n", pid);
        char command[50];
        sprintf(command, "cat /proc/%d/maps", pid);
        system(command);
    
    return 0; 
}