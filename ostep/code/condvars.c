/***
 * 
 * Condition variables examples
 * p. 309 - 311
 * 
***/

#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <stdlib.h>

static int volatile ready = 0; 

int main(int argc, char* argv[])
{

    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t cond = PTHREAD_COND_INITIALIZER; 

    pthread_mutex_lock(&lock); 
    /* This while-loop is used instead of an if-statement to re-check the condition. */
    while (ready == 0)
    {
        /* Puts the current thread to sleep and waits for another thread to signal it. */
        /* Typically that something in the program has changed that the sleeping thread cares about. */
        /* This method takes the lock as 2nd argument, because it releases it during sleep to be able to singal the thread.. */
        pthread_cond_wait(&cond, &lock); 
    }
    pthread_mutex_unlock(&lock); 

    /* The code to wake thread, which would run in some other thread: */
    /* When changing the global shared var, and signal the thread,we acquire the lock */
    /* This, to avoid race condition. */
    pthread_mutex_lock(&lock);
    ready = 1; 
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&lock);

    return 0; 
}

/* Conclusion: Don't be lazy, ALWAYS use condition variables */
/* - less buggy, - less costly - less error prone  - easy to use */


/* When using the pthread library, we must explicitly link it with -lpthread flag. */