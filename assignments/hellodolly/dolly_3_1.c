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
        int child = getpid(); 
        printf("I'm the child %d in group %d\n", child, getpgid(child));
    } else
    {
        int parent = getpid(); 
        printf("I'm the parent %d in group %d\n", parent, getpgid(parent));
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

/* Running 'ps -fp $$' get is the parent process ID of the shell (/bin/bash)*/
/* Running 'ps -fp <shell_pid> give us the process ID of the terminal (1) (/usr/bin/qterminal)*/
/* Running 'ps -fp <terminal_pid> give us the process ID of the Root (0). (/sbin/init splash) *root* */

/* So.. Parent of the mother process is the bash-process and the process tree goes back to the root. */

/* A parent and a child belongs to the same group. */