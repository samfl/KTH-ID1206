#include <stdio.h> /* prrocedure 'printf()' etc. */
#include <unistd.h> /* Required by the procedure 'fork()' */
#include <sys/types.h> /* Type 'pid_t' is defined here */
#include <sys/wait.h> /* Required by the procedure 'wait()' */

int main (void) 
{
    printf("Don't get in my way. \n");
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

/* When the parent process crashes, the new parent gets the pid 1 (pid of root process) */

/* A daemon is a process running in the background detached from ay controlling terminal */

/* Many tasks performed by an OS is performed by daemons, they consume. They keep track of e.g. network interfaces, USB or scheduled tasks. */

/* A program is executed with the exec() family oof system calls - Looks for programs in the PATH environment. */

/* Note that if we execute boba.c we need './' reference before if its not included in PATH. */

/* Redirect the output of a program to a file quotes.txt: ./boba > quotes.txt */