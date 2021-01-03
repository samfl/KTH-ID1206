#include "green.h"

#define LOOP 10
#define MAX 10

int flag = 0; 
int count = 0; 
green_cond_t cond; 
green_mutex_t mutex; 
pthread_cond_t p_cond; 
pthread_mutex_t p_mutex;

/* Producer / Consumer */
int buffer[MAX];
int fill_ptr = 0; 
int use_ptr = 0;
int loops = LOOP;
green_cond_t empty;
green_cond_t fill; 

void* test_yield(void* arg);
void* test_condv(void* arg);
void* test_interrupts(void* arg);
void* test_interrupts_fail(void* arg);
void* test_mutex(void* arg);
void* test_condv_mutex(void* arg);
void put(int val);
int get(void); 
void* producer(void* arg);
void* consumer(void* arg);

void* test_p_yield(void* arg);
void* test_p_condv(void* arg);
void* test_p_mutex(void* arg);
void* test_p_condv_mutex(void* arg);
void* p_producer(void* arg);
void* p_consumer(void* arg);

int main(void)
{
    clock_t start_green = clock();

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
    green_create(&g0, test_interrupts, &a0);
    green_create(&g1, test_interrupts, &a1);
    // green_create(&g0, test_interrupts_fail, &a0);
    // green_create(&g1, test_interrupts_fail, &a1);
    // green_create(&g0, test_mutex, &a0);
    // green_create(&g1, test_mutex, &a1);
    // green_create(&g0, test_condv_mutex, &a0);
    // green_create(&g1, test_condv_mutex, &a1);
    // green_create(&g0, producer, &a0);
    // green_create(&g1, consumer, &a1);

    printf("Count: %d\n", count);

    print_queue();

    green_join(&g0, NULL);
    green_join(&g1, NULL); 

    clock_t end_green = clock();
    printf("Count: %d\n", count);

    print_queue();


    /* For pthread tests */
    printf("\n Pthread-procedures: \n");   
    clock_t start_pthread = clock();
    pthread_t p0, p1;
    int a2 = 0;
    int a3 = 1; 
    count = 0; 
    pthread_cond_init(&p_cond, NULL); 
    pthread_mutex_init(&p_mutex, NULL); 

    printf("Count: %d\n", count);  

    // pthread_create(&p0, NULL, test_p_yield, &a2);
    // pthread_create(&p1, NULL, test_p_yield, &a3);
    // pthread_create(&p0, NULL, test_p_condv, &a0);
    // pthread_create(&p1, NULL, test_p_condv, &a1);
    // pthread_create(&p0, NULL, test_p_mutex, &a0);
    // pthread_create(&p1, NULL, test_p_mutex, &a1);
    // pthread_create(&p0, NULL, test_p_condv_mutex, &a0);
    // pthread_create(&p1, NULL, test_p_condv_mutex, &a1);
    // pthread_create(&p0, NULL, p_producer, &a0);
    // pthread_create(&p1, NULL, p_consumer, &a1);

    pthread_join(&p0, NULL);
    pthread_join(&p1, NULL); 
    clock_t end_pthread = clock();

    printf("Count: %d\n", count);  

    clock_t diff_green = start_green - end_green; 
    clock_t diff_pthread = start_pthread - end_pthread; 
    
    printf("Green-thread: %d, Pthread: %d \n", diff_green, diff_pthread);
    printf("done\n");

    return 0; 
}

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

void* test_p_yield(void* arg)
{
    int i = *(int*) arg; 
    int loop = LOOP; 
    while (loop > 0) 
    {
        loop--;
    }
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
            green_cond_wait(&cond, NULL); 
        }
    }
    return (void*) 0;
}

void* test_p_condv(void* arg)
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
            pthread_cond_signal(&p_cond);
        } else
        {
            pthread_cond_wait(&p_cond, &p_mutex); 
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
        } 
    }
    return (void*) 0;
}

/* Shared resource, global shared counter */
void* test_interrupts_fail(void* arg)
{
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

void* test_mutex(void* arg)
{
    int id = *(int*) arg; 
    int loop = LOOP; 

    while (loop > 0)
    {
        // printf("thread %d: %d\n", id, loop);
        loop--;

        green_mutex_lock(&mutex);
        count++;
        green_mutex_unlock(&mutex);
    }

    return (void*) 0;
}

void* test_p_mutex(void* arg)
{
    int id = *(int*) arg; 
    int loop = LOOP; 

    while (loop > 0)
    {
        // printf("thread %d: %d\n", id, loop);
        loop--;

        pthread_mutex_lock(&p_mutex);
        count++;
        pthread_mutex_unlock(&p_mutex);
    }

    return (void*) 0;
}

/* Just to verify that the green_cond_wait procedure is correctly implemented (With Mutex) - Final touch */
void* test_condv_mutex(void* arg)
{
    int id = *(int*) arg; 
    int loop = LOOP; 

    while (loop > 0)
    {
        printf("thread %d: %d\n", id, loop);
        green_mutex_lock(&mutex);
        while (flag != id)
        {
            green_mutex_unlock(&mutex);
            green_cond_wait(&cond, &mutex);
            green_mutex_lock(&mutex);
        }
        flag = (id + 1) % 2;
        green_cond_signal(&cond);
        green_mutex_unlock(&mutex);
        loop--;
    }
    return (void*) 0;
}

/* Just to verify that the green_cond_wait procedure is correctly implemented (With Mutex) - Final touch */
void* test_p_condv_mutex(void* arg)
{
    int id = *(int*) arg; 
    int loop = LOOP; 

    while (loop > 0)
    {
        printf("thread %d: %d\n", id, loop);
        pthread_mutex_lock(&p_mutex);
        while (flag != id)
        {
            pthread_mutex_unlock(&p_mutex);
            pthread_cond_wait(&p_cond, &p_mutex);
            pthread_mutex_lock(&p_mutex);
        }
        flag = (id + 1) % 2;
        pthread_cond_signal(&p_cond);
        pthread_mutex_unlock(&p_mutex);
        loop--;
    }
    return (void*) 0;
}

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
        green_mutex_lock(&mutex);
        while (count == 0)
        {
            green_cond_wait(&cond, &mutex);
        }
        put(i);
        green_cond_signal(&cond); 
        green_mutex_unlock(&mutex);
    }
    return (void*) 0; 
}

/**/
void* consumer(void* arg)
{
    int i;
    for (i = 0; i < loops; i++)
    {
        green_mutex_lock(&mutex);
        while (count == 0)
        {
            green_cond_wait(&cond, NULL);
        }
        int tmp = get(); 
        green_cond_signal(&cond); 

        green_mutex_unlock(&mutex);
        printf("%d\n", tmp);
    }
    return (void*) 0; 
}

