#include <stdio.h> /* prrocedure 'printf()' etc. */
#include <unistd.h> /* Required by the procedure 'fork()' */
#include <sys/types.h> /* Type 'pid_t' is defined here */
#include <sys/wait.h> /* Required by the procedure 'wait()' */

int main (void) 
{
    int pid;
    int x = 123;
    pid = fork();

    /* In parent - pid has a value -> "I'm the child XXXXX */
    /* In child - pid is 0 -> My child is called 0 */
    /* A process can produce a value when it terminates, this value can be picked up by the mother process: */
    if (pid == 0)
    {   
        printf("Child: x is %d and the memory addr is 0x%p\n", x, &x);
        x = 42; 
        sleep(1); 
        printf("Child: x is %d and the memory addr is 0x%p\n", x, &x);
        return 42;
    } else
    {
       printf("Mother: x is %d and the memory addr is 0x%p\n", x, &x);
       x = 13; 
       sleep(1);
       printf("Mother: x is %d and the memory addr is 0x%p\n", x, &x);
       wait(NULL); 
    }

    return 0; 
}

/* Different OS has chosen different strategies and e.g. Windows have 
   chosen to provide a procedure that creates a new process that is 
   independant of the mother process.
*/

/* Both mother and child process have the same memory address (virtually memory). */
/* However, each process have its own memory space separated from one another. */
/* In physical memory their addresses will be different however, the process think it has that virtual address. */