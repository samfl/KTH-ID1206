#include <stdio.h>
#include <unistd.h>
#include "mymal.h"

int main(void)
{

    unsigned int* mem = (unsigned int*) balloc(8);
    unsigned int* mem2 = (unsigned int*) balloc(16);
    unsigned int* mem3 = (unsigned int*) balloc(32);
    unsigned int* mem4 = (unsigned int*) balloc(128);
    unsigned int* mem5 = (unsigned int*) balloc(256);
    unsigned int* mem6 = (unsigned int*) balloc(4000);
    unsigned int* mem7 = (unsigned int*) balloc(80000);

    *mem = 1;
    *mem2 = 22;
    *mem3 = 333;
    *mem4 = 4444;
    *mem5 = 55555;
    *mem6 = 666666;

    printf("At address: %p, we have value: %d\n", mem, *mem);
    printf("At address: %p, we have value: %d\n", mem2, *mem2);
    printf("At address: %p, we have value: %d\n", mem3, *mem3);
    printf("At address: %p, we have value: %d\n", mem4, *mem4);
    printf("At address: %p, we have value: %d\n", mem5, *mem5);
    printf("At address: %p, we have value: %d\n", mem6, *mem6);

    bree(mem);
    bree(mem2);
    bree(mem3);
    bree(mem4);
    bree(mem5);
    bree(mem6);

    return 0; 
}