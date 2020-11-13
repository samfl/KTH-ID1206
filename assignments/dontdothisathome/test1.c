#include <stdio.h>
#include <signal.h>
#include <unistd.h>

int volatile count;

void handler(int sig)
{
    printf("signal %d ouch that hurt\n", sig);
    count++;
}

int main(void)
{
    struct sigaction sa; 

    int pid = getpid();

    printf("ok, let's go, kill me (%d) if you can!\n", pid);

    sa.sa_handler = handler; 
    sa.sa_flags = 0; 
    sigemptyset(&sa.sa_mask);

    if(sigaction(SIGINT, &sa, NULL) != 0)  
    {
        return(1);
    }

    while(count != 4)
    {

    }

    printf("I've had enough!\n");

    return(0);
}

/*
**********************************************************************
----------------------------------------------------------------------
test_1 LOG:
----------------------------------------------------------------------

> kill SIGINT <PID>
> kill 2 <PID>
Result: Terminated

> kill -SIGINT <PID>
> kill -2 <PID>
Result: Will trigger the interrupt handler (handler).

> kill -SIGKILL <PID>
> kill -9 <PID>
Result: Killed

> kill -SIGTERM <PID>
> kill -15 <PID>
Result: Terminated

> kill -19 <PID>
> kill -SIGSTOP <PID>
Result: [1]+ Stopped ./test1

> kill -6 <PID>
> kill -SIGABRT <PID>
Result: Aborted (core dumped)

> kill -3 <PID>
> kill -SIGQUIT <PID>
Result: Quit (core dumped)

> man kill
Result: "Particular useful signals include HUP, INT, KILL, STOP, CONT and 0"

> man sigaction
Result: "A system call used to change the action taken by a process on receipt of a specific signal." 
        Describes sa_handler, sa_mask etc. 
        #include <signal.h>

**********************************************************************
*/ 