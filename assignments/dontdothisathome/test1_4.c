#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

int volatile done;

void handler(int sig, siginfo_t *siginfo, void *context)
{
    printf("signal %d was caught \n", sig);
    printf("Your UID is %d\n", siginfo->si_uid);
    printf("Your PID is %d\n", siginfo->si_pid);
    done = 1; 
}

int not_so_good(void)
{
    int x = 0; 
    return 1 % x; 
}

int main(void)
{
    struct sigaction sa; 

    int pid = getpid(); 
    
    printf("Ok, let's go - Kill me (%d). \n", pid);

    /* we're using the more elaborated sigaction handler */
    sa.sa_flags = SA_SIGINFO;
    sa.sa_sigaction = handler; 
    sigemptyset(&sa.sa_mask);

    /* and now we catch ... FPE signals */
    if (sigaction(SIGINT, &sa, NULL) != 0)
    {
        return(1); 
    }

    while(!done)
    {

    }

    printf("Told yoo so! \n");
    return(0);
}

/*

When this process is Interrupted, it will return info about the 
process that sent the interrupt signal. 

*/