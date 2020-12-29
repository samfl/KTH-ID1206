#include "green.h"

#define LOOP 1000000

int flag = 0; 
int count = 0; 
green_cond_t cond; 

void* test_yield(void* arg)
{
    int i = *(int*) arg; 
    int loop = LOOP; 
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
    int loop = LOOP; 

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

void* test_interrupts(void* arg)
{
    int id = *(int*) arg; 
    int loop = LOOP; 

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

/* Shared resource, global shared counter */
void* test_interrupts_fail(void* arg)
{
    int id = *(int*) arg; 
    for (int i = 0; i < 1000000; i++)
    {   
        /* LOCK NEEDED */
        int tmp = count; 

        /* Create some delay between Read / Write */
        int dummy = 0; 
        if (tmp > 1000)
        {
            ++dummy;
        }
        tmp++; 
        /* ****************************************/

        count = tmp; 
        /* UNLOCK NEEDED */
    }
    return (void*) 0;
}

#define MAX 10
int buffer[MAX];
int fill_ptr = 0; 
int use_ptr = 0;
int loops = LOOP;
green_cond_t empty;
green_cond_t fill; 

/* Helpers for Producer / Consumer problem */
void put(int val)
{
    buffer[fill_ptr] = val; 
    fill_ptr = (fill_ptr + 1) % MAX; 
    count++; 
    return; 
}

/* Helpers for Producer / Consumer problem */
int get(void)
{
    int tmp = buffer[use_ptr];
    use_ptr = (use_ptr + 1) % MAX;
    count--; 
    return tmp; 
}

/* */
void* producer(void* arg)
{
    int i;
    for (i = 0; i < loops; i++)
    {
        /* LOCK HERE */
        while (count == 0)
        {
            green_cond_wait(&cond);
        }
        put(i);
        green_cond_signal(&cond); 
        /* UNLOCK HERE*/
    }
    return (void*) 0; 
}

/**/
void* consumer(void* arg)
{
    int i;
    for (i = 0; i < loops; i++)
    {
        /* LOCK HERE */
        while (count == 0)
        {
            green_cond_wait(&cond);
        }
        int tmp = get(); 
        green_cond_signal(&cond); 

        /* UNLOCK HERE*/
        printf("%d\n", tmp);
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
    printf("Count: %d\n", count);

    // green_create(&g0, test_yield, &a0);
    // green_create(&g1, test_yield, &a1);
    // green_create(&g0, test_condv, &a0);
    // green_create(&g1, test_condv, &a1);
    // green_create(&g0, test_interrupts, &a0);
    // green_create(&g1, test_interrupts, &a1);
    green_create(&g0, test_interrupts_fail, &a0);
    green_create(&g1, test_interrupts_fail, &a1);

    printf("Count: %d\n", count);

    print_queue();

    green_join(&g0, NULL);
    green_join(&g1, NULL); 
    printf("Count: %d\n", count);

    print_queue();
    
    printf("done\n");

    return 0; 
}
