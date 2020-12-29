#include "green.h"

#define FALSE 0
#define TRUE 1
#define STACK_SIZE 4096

/* Function Prototypes */
static void enqueue(green_t* thread);
static green_t* dequeue(void);

/* Varible Declarations */
static ucontext_t main_cntx = {0};
static green_t main_green = {&main_cntx, NULL, NULL, NULL, NULL, NULL, FALSE};
static green_t* running = &main_green;
static green_t* queue_head = NULL;
static green_t* queue_tail = NULL;

/* Enqueue a thread from the back (tail) */
static void enqueue(green_t* thread)
{
	if (queue_head == NULL)
	{
		queue_head = thread;
		queue_tail = thread;
		return;
	} else
	{
		queue_tail->next = thread;
		queue_tail = thread;
		return;
	}
}

/* Dequeue a thread from the front (head) */
static green_t* dequeue(void)
{
	if (queue_head == NULL)
	{
		return NULL;
	} else
	{
		green_t* dequeued = queue_head;
		queue_head = queue_head->next;
		dequeued->next = NULL;
		return dequeued;
	}
}

/* Print out content of the ready-queue */
void print_queue(void)
{
	green_t* iter = queue_head;
	if (iter == NULL)
	{
		printf("Ready-queue empty!\n");
		return;
	} else
	{
		printf("Ready-queue:");
		while (iter != NULL)
		{
			printf("\t thread: %p\n", iter->arg);
			iter = iter->next;
		}
		return;
	}
}

/* '__attribute__' allows us to specify 'constructor' */
/* 'constructor' causes the func to be automatically called execution enters main() */
static void init() __attribute__((constructor));

/* Initializes the main_cntx */
/* When we call the scheduling function for the first time the running thread will be properly initialized. */
void init()
{
    /* Initializing the structure, pointed at ucp, to the currently active context. */
    getcontext(&main_cntx);
    return;
}

/* Function that will start execution of the real function. */
/* When returning from the call, it will terminate the thread. */
void green_thread(void)
{
    green_t* this = running;  

    void* result = (*this->fun)(this->arg);

    // Place waiting (joining) thread in ready queue.
    if (this && (this->join != NULL))
    {
        enqueue(this->join);
    }

    // Save result of execution.
    this->retval = result; 
    
    // We're a zombie.
    this->zombie = TRUE; 

    // Find the next thread to run.
    green_t* next = dequeue();

    running = next; 
    setcontext(running->context);
    return;
}

/* Initializes a green thread. */
int green_create(green_t* new, void* (*fun) (void*), void* arg)
{
    ucontext_t* cntx = (ucontext_t*) malloc(sizeof(ucontext_t));
    /* Initializing the structure, pointed at ucp, to the currently active context. */
    getcontext(cntx);

    /* makecontext() function modifies the context pointed to by ucp (via getcontext)  */
    /* Caller must allocate a new stack for this context and assign its address to ucp->uc_stack before */
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
    enqueue(new);

    return 0; 
}

/* Suspends the current thread and selects a new thread for execution. */
int green_yield(void)
{
    green_t* susp = running;

    // Add susp to ready queue. 
    enqueue(susp);

    // Select the next thread for execution
    green_t* next = dequeue();

    running = next;
    /* swapcontext() function saves the current context, in the structure pointer at by oucp, activates the context pointer at bt ucp */
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
        thread->join = susp; 

        // Select the next thread for execution.
        green_t* next = dequeue();

        running = next; 
        /* swapcontext() function saves the current context, in the structure pointer at by oucp, activates the context pointer at bt ucp */
        swapcontext(susp->context, next->context);
    }

    // Collect result. 
    res = thread->retval; 

    // Free context. 
    free(thread->context);

    return 0; 
}

/* Initialize the green_cond_var */
void green_cond_init(green_cond_t* condv)
{
    condv->head = NULL;
    return;
}

/* Suspend current thread on the condtition */
void green_cond_wait(green_cond_t* condv)
{
    green_t* susp = running;
    green_t* curr = condv->head;

    if (curr != NULL)
    {
        while (curr->next != NULL)
        {
            curr = curr->next;
        }
        curr->next = susp;
    } else
    {
        condv->head = susp;
    }

    green_t* next = dequeue();
    running = next;
    swapcontext(susp->context, next->context);

    return;
}

/* Move the first suspended thread to the ready queue */
void green_cond_signal(green_cond_t* condv)
{
    if (condv->head != NULL)
    {
        enqueue(condv->head);
        condv->head = condv->head->next;
    }
    return;
}
