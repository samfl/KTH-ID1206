#include <stdio.h> /* prrocedure 'printf()' etc. */
#include <unistd.h> /* Required by the procedure 'fork()' */
#include <sys/types.h> /* Type 'pid_t' is defined here */

int main (void) 
{
    int pid = fork();

    printf("pid = %d\n", pid);

    return 0; 
}

/* Double prints will be performed (one for each process). */
/* Fork will return the child-pid to the parent */
/* Fork will return value '0' to the child */
/* Fork will return value '-1' to the parent upon failure (no 
   new process will be created) */
/* Fork will duplicate the process and the execution will 
   continue to in both copies. */ 