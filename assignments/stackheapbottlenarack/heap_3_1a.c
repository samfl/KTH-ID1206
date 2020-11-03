#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{   

    int pid = getpid();

    char* heap = malloc(20);
    *heap = 0x61;
    printf("heap pointing to: 0x%x\n", *heap);

    free(heap);

    char* foo = malloc(20);
    *foo = 0x62;
    printf("foo pointing to: 0x%x\n", *foo);

    /* Danger ahead */
    *heap = 0x63;
    printf("or is it pointing to: 0x%x\n", *foo);

    /* Foo was assigned the address 0x63.. Since the memory for heap was deallocated above. */

    /* Memory trace */
    back: 
        printf(" back: %p \n", &&back);
        printf(" \n\n /proc/%d/maps \n\n", pid);
        char command[50];
        sprintf(command, "cat /proc/%d/maps", pid);
        system(command);
    
    return 0; 
}