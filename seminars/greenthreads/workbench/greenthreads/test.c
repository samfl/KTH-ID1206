#include "green.h"

void* test(void* arg)
{
    int i = *(int*) arg; 
    int loop = 4; 
    while (loop > 0) 
    {
        printf("thread %d: %d\n",  i, loop);
        loop--;
        green_yield(); 
    }
    return (void*)0;
}

int main(void)
{
    green_t g0, g1; 
    int a0 = 0; 
    int a1 = 1; 
    print_queue();
    green_create(&g0, test, &a0);
    print_queue();
    green_create(&g1, test, &a1);
    print_queue();

    green_join(&g0, NULL);
    print_queue();
    green_join(&g1, NULL); 
    print_queue();
    
    printf("done\n");

    return 0; 
}
