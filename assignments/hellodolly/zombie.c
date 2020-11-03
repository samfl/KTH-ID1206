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
    /* What happens if we terminate the child process before calling wait? -> ZOMBIE */ 
    /* A zombie-process is a process which has terminated but whose parent process has not been informed. */
    
    if (pid == 0)
    {
        printf("Check the status\n");
        sleep(10);
        printf("and again\n");
        return 123;
    } else 
    {
        sleep(20); 
        int res; 
        wait(&res);
        printf("The result was %d\n", WEXITSTATUS(res));
        printf("and again\n");
        sleep(10);
    }
    return 0; 
}

/* Different OS has chosen different strategies and e.g. Windows have 
   chosen to provide a procedure that creates a new process that is 
   independant of the mother process.
*/

/* Status of processes can be checked with the command 'ps' */
/* A process can be ran in the background by appending the char '&' to the end */