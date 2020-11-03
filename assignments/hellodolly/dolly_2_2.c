#include <stdio.h> /* prrocedure 'printf()' etc. */
#include <unistd.h> /* Required by the procedure 'fork()' */
#include <sys/types.h> /* Type 'pid_t' is defined here */
#include <sys/wait.h> /* Required by the procedure 'wait()' */

int main (void) 
{
    int pid = fork();

    /* In parent - pid has a value -> "I'm the child XXXXX */
    /* In child - pid is 0 -> My child is called 0 */
    /* A process can produce a value when it terminates, this value can be picked up by the mother process: */
    if (pid == 0)
    {
        return 42;
    } else
    {
        int res; 
        wait(&res); /* Waiting for the child, grabbing the return value and saving it to the mem-location of 'res'. */
        printf("The result was: %d\n", WEXITSTATUS(res));
    }

    return 0; 
}

/* Different OS has chosen different strategies and e.g. Windows have 
   chosen to provide a procedure that creates a new process that is 
   independant of the mother process.
*/