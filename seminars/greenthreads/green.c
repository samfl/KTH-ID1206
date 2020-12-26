#include <stdlib.h>
#include <ucontext.h>
#include <assert.h>
#include "green.h"

#define FALSE 0
#define TRUE 1

#define STACK_SIZE 4096

static ucontext_t main_cntx = {0};
static green_t main_green = {&main_cntx, NULL, FALSE}
static green_t* running = &main_green;

static void init() __attribute__((constructor));

/* Initializes the main_cntx so when we call the scheduling function for the first time the running thread will be properly initialized. */
void init()
{
    getcontext(&main_cntx);
}

void green_thread(void)
{
    green_t* this = running;  

    void* result = (*this->fun)(this->arg);

    // Place waiting (joining) thread in ready queue. 

    // Save result of execution.

    // We're a zombie.

    // Find the next thread to run.

    running = next; 
    setcontext(next->context);
}

/* Initializes a green thread. */
int green_create(green_t* new, void* (*fun) (void*), void* arg)
{
    ucontext_t* cntx = (ucontext_t*) malloc(sizeof(ucontext_t));
    getcontext(cntx);

    void* stack = malloc(STACK_SIZE);

    cntx->uc_stack.ss_sp = stack; 
    cntx->uc_stack.ss_size = STACK_SIZE;
    makecontext(cntx, green_thread, 0);

    new->context = cntx; 
    new->fun = fun; 
    new->arg = arg; 
    new->next = NULL; 
    new->join = NULL; 
    new->retval = NULL; 
    new->zombie = FALSE; 

    // Add new to the ready queue.  

    return 0; 
}

/* Suspends the current thread and selects a new thread for execution. */
int green_yield(void)
{
    green_t* susp = running; 

    // Add susp to ready queue. 

    // Select the next thread for execution

    running = next;
    swapcontext(susp->context, next->context);

    return 0; 
}

/* The current thread is suspended waiting for a thread to terminate. */
int green_join(green_t* thread, void** res)
{

    if (!thread->zombie)
    {
        green_t* susp = running; 

        // Add as joining thread.

        // Select the next thread for execution.

        running = next; 
        swapcontext(susp->context, next->context);
    }

    // Collect result. 

    // Free context. 

    return 0; 
}