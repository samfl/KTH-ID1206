#include "green.h"

int flag = 0;
green_cond_t cond;

void* test_thread(void* arg)
{
    int i = *(int*) arg; 
    int loop = 4; 
    while (loop > 0) 
    {
        printf("thread %d: %d\n",  i, loop);
        loop--;
        green_yield(); 
    }
    return (void*) 0;
}

void* test_condv(void* arg)
{
    int id = *(int*) arg;
    int loop = 4;

    while (loop > 0)
    {
        if (flag == id)
        {
            printf("thread %d: %d\n", id, loop);
            loop--;
            flag = (id + 1) % 2;
            green_cond_signal(&cond);
        } else
        {
            green_cond_wait(&cond);
        }
    }
    return (void*) 0;
}

int main(void)
{
    green_t g0, g1; 
    int a0 = 0; 
    int a1 = 1; 

    green_cond_init(&cond);

    print_queue();

    // green_create(&g0, test_thread, &a0);
    // green_create(&g1, test_thread, &a1);
    green_create(&g0, test_condv, &a0);
    green_create(&g1, test_condv, &a1);

    print_queue();

    green_join(&g0, NULL);
    green_join(&g1, NULL); 

    print_queue();
    
    printf("done\n");

    return 0; 
}
