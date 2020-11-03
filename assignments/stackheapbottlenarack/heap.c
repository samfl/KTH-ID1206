#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
    int pid = getpid();

    char* heap = malloc(20);

    printf("The heap variable at: %p\n", &heap);
    printf("Pointing to: %p\n", heap);

    back: 
        printf(" back: %p \n", &&back);
        printf(" \n\n /proc/%d/maps \n\n", pid);
        char command[50];

        sprintf(command, "cat /proc/%d/maps", pid);
        system(command);

    return 0; 
}