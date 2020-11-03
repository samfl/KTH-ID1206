#include <stdio.h> /* prrocedure 'printf()' etc. */
#include <unistd.h> /* Required by the procedure 'fork()' */
#include <sys/types.h> /* Type 'pid_t' is defined here */
#include <sys/wait.h> /* Required by the procedure 'wait()' */

int main (void) 
{
    int pid = fork();

    /* In parent - pid has a value -> "I'm the child XXXXX */
    /* In child - pid is 0 -> My child is called 0 */
    if (pid)
    {
        printf("I'm the child %d\n", getpid());
        sleep(1); /* Delay, 1 second */
    } else
    {
        printf("My child is called %d\n", pid);
        wait(NULL); /* Waiting for the child (any child it has spawned) process to terminate */
        printf("My child has terminated\n");
    }

    printf("This is the end (%d)\n", getpid());

    return 0; 
}

/* Different OS has chosen different strategies and e.g. Windows have 
   chosen to provide a procedure that creates a new process that is 
   independant of the mother process.
*/