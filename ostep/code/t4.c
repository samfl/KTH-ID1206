/***
 * 
 * Simpler Argument Passing to a Thread
 * Figure 27.3, p.307
 * 
***/

#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <stdlib.h>

void* my_thread(void* arg)
{
    int m = (int) arg;
    printf("%d \n", m);
    return (void*) (arg + 1);
}

int main(int argc, char* argv[])
{
    pthread_t p;
    int rc, m; 

    /* Instead of calling both pthread_create and pthread_join, there is a procedural call. */
    pthread_create(&p, NULL, my_thread, (void*) 100);
    pthread_join(p, (void**) &m);
    printf("returned %d\n", m);

    return 0; 
}