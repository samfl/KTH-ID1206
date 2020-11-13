#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

int volatile count;

void handler(int sig)
{
    printf("signal %d was caught \n", sig);
    exit(1); 
    return; 
}

int not_so_good(void)
{
    int x = 0; 
    return 1 % x; 
}

int main(void)
{
    struct sigaction sa; 

    printf("Ok, let's go - I'll catch my own error.\n");

    sa.sa_handler = handler; 
    sa.sa_flags = 0; 
    sigemptyset(&sa.sa_mask);

    /* and now we catch ... FPE signals */
    sigaction(SIGFPE, &sa, NULL);

    not_so_good(); 

    printf("Will probably not write this.\n");
    return(0);
}

/*
1.  The zero-division is first detected by the hardware; when the instruction is executed,
    an exception is raised.  
2.  CPU will then look in the Interrupt Descriptior Table.
3.  It will jump to the location in memory the hopefully will contain some code to sove the problem. 
4.  The code is part of the kernel, so the kernel decides what to do. 
5.  The user process must of course be interrupted.
6.  If the process has registered its own SIGFPE handler, control is passed to that function.
7.  If not, the default procedure is to kill the process. 
*/