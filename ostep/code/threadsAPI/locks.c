/***
 * 
 * Locks examples
 * p. 307 - 310
 * 
***/

#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <stdlib.h>

typedef struct __timespec_t
{
    int s; 
    int m; 
    int h; 
} timespec_t; 

int main(int argc, char* argv[])
{

    timespec_t t = {0, 40, 13};
    /* To be able to use locks, we must properly initialize them: */
    /* This can be done statically: */
    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; 

    /* And dynamically (during runtime): */
    int rc = pthread_mutex_init(&lock, NULL);

    /* When we are done with a lock, a corresponding call to destroy the lock shall be made. */
    rc = pthread_mutex_destroy(&lock); 

    /* For lock acqustition, these procedures can be used: */
    /* These procedures also deals with failure (error codes) */
    rc = pthread_mutex_trylock(&lock);
    rc = pthread_mutex_timedlock(&lock, &t);
    assert(rc == 0);

    /* The general Idea: */
    pthread_mutex_t lock2 = PTHREAD_MUTEX_INITIALIZER; 
    pthread_mutex_lock(&lock2); 
    /* A critical section, e.g. a global shared counter */
    pthread_mutex_unlock(&lock2);

    return 0; 
}