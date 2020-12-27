/***
 * 
 * Sharing Data: Uh Oh
 * Figure 26.6, p.292
 * 
***/

#include <stdio.h>
#include <pthread.h>

static volatile int counter = 0; 

void* my_thread(void* arg)
{
    printf("%s\n", (char*) arg);
    int i; 
    for (i = 0; i < 1e7; i++)
    {
        counter++; 
    }
    printf("%s done\n", (char*) arg);
    return NULL; 
}

int main(int argc, char* argv[])
{
    pthread_t p1, p2; 
    printf("main: begin (counter = %d)\n", counter);
    pthread_create(&p1, NULL, my_thread, "A");
    pthread_create(&p2, NULL, my_thread, "B");
    pthread_join(p1, NULL);
    pthread_join(p2, NULL);
    printf("main: done with both (counter = %d)\n", counter);

    return 0; 
}

/* Conclusion: counter is a global shared variable, and thus the increments will take the wrong values sometimes due to a race condition */
/* This happens because the compiler generates an instruction sequence */
/* Since each thread running has its own private registers (virtualized by the context-switch saving and restoring them) */
/* ... code for incrementing the counter can be run twice but the actual value of the shared resource is only incremented once. */
/* We have encountered a race condition: result is dependent on the timing execution of code. */
/* Due concurrency and these race conditions, we must have atomicity and synchrnization primitives. */