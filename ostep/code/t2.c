/***
 * 
 * Creating a Thread
 * Figure 27.1, p.305
 * 
***/

#include <stdio.h>
#include <pthread.h>

typedef struct __myarg_t
{
    int a; 
    int b; 
} myarg_t;

void* my_thread(void* arg)
{
    myarg_t* m = (myarg_t*) arg;
    printf("%d %d \n", m->a, m->b);
    return NULL; 
}

int main(int argc, char* argv[])
{
    pthread_t p;
    int rc;

    myarg_t args; 
    args.a = 10;
    args.b = 20; 
    rc = pthread_create(&p, NULL, my_thread, &args);
    rc = pthread_join(p, NULL);
    return 0; 
}