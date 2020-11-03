#include <stdio.h> /* prrocedure 'printf()' etc. */
#include <unistd.h> /* Required by the procedure 'fork()' */
#include <sys/types.h> /* Type 'pid_t' is defined here */

int main (void) 
{
    int pid = fork();

    /* In parent - pid has a value -> "I'm the child XXXXX */
    /* In child - pid is 0 -> My child is called 0 */
    if (pid)
    {
        printf("I'm the child %d\n", getpid());
    } else
    {
        printf("My child is called %d\n", pid);
    }

    printf("That's it %d\n", getpid());

    return 0; 
}

/* Different OS has chosen different strategies and e.g. Windows have 
   chosen to provide a procedure that creates a new process that is 
   independant of the mother process.
*/