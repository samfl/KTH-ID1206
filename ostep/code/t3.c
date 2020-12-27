/***
 * 
 * Waiting for Thread Completion
 * Figure 27.2, p.306
 * 
***/

#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <stdlib.h>

typedef struct __myarg_t
{
    int a; 
    int b; 
} myarg_t;

typedef struct __myret_t
{
    int x; 
    int y; 
} myret_t;

void* my_thread(void* arg)
{
    myarg_t* m = (myarg_t*) arg;
    printf("%d %d \n", m->a, m->b);

    myret_t* r = malloc(sizeof(myret_t));
    r->x = 1;
    r->y = 2;
    return (void*) r;
}

int main(int argc, char* argv[])
{
    pthread_t p;
    myret_t* m;

    myarg_t args = {10, 20}; 
    /* Takes a pointer to the thread, attributes (NULL), start routine, arguments to thread */
    pthread_create(&p, NULL, my_thread, &args);
    /* Takes the thread and its return value as arguments */
    pthread_join(p, (void**) &m);
    printf("returned %d %d\n", m->x, m->y);

    return 0; 
}