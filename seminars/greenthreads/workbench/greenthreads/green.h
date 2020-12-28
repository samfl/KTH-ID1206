#ifndef _GREEN_H
#define _GREEN_H

#include <ucontext.h>
#include <stdio.h>

/* Representation of our green thread. */
typedef struct green_t
{
    ucontext_t* context;
    void* (*fun) (void*);
    void* arg;
    struct green_t* next;
    struct green_t* join; 
    void* retval; 
    int zombie;
} green_t;

/* Enqueue a thread to the ready-queue */
void enqueue(green_t* thread);

/* Dequeue a thread from the ready-queue */
green_t* dequeue(void);

/* Print threads in the ready-queue */
void print_queue(void);

/* Initializes a green thread. */
int green_create(green_t* thread, void* (*fun)(void*), void* arg);

/* Suspends the current thread and selects a new thread for execution. */
int green_yield(void);

/* The current thread is suspended waiting for a thread to terminate. */
int green_join(green_t* thread, void** val); 

#endif
