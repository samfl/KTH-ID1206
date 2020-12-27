/***
 * 
 * Simple Thread Creation
 * Figure 26.2, p.290
 * 
***/

#include <stdio.h>
#include <assert.h>
#include <pthread.h>

void* my_thread(void* arg)
{
    printf("%s\n", (char*) arg);
    return NULL; 
}

int main(int argc, char* argv[])
{
    pthread_t p1, p2; 
    int rc; 

    printf("main: begin\n");

    /* Creates a new thread  in the calling process. */
    /* A thread may run directly upon creation */
    rc = pthread_create(&p1, NULL, my_thread, "A");

    /* Aborts the program if the assertion is false. */
    assert(rc == 0);

    /* The execution ordering is dependent on the OS scheduler and what thread it decides to run at a given point. */
    rc = pthread_create(&p2, NULL, my_thread, "B");
    assert(rc == 0); 

    /* Join waits for the threads to terminate */
    /* If the thread has already terminated, this procedure will return immediately */
    rc = pthread_join(p1, NULL);
    assert(rc == 0);

    rc = pthread_join(p2, NULL);
    assert(rc == 0);

    printf("main: end\n");

    return 0; 
}

/* Conclusion: Both thread A & B could be run first. They could both be executed directly upon creation or upon the waiting 'join' procedure. */