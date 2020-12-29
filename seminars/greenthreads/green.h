#ifndef _GREEN_H
#define _GREEN_H

#include <stdlib.h>
#include <ucontext.h>
#include <assert.h>
#include <stdio.h>
#include <ucontext.h>
#include <signal.h>
#include <sys/time.h>

/* Representation of our green thread. */
typedef struct _green_t
{
    ucontext_t* context;
    void* (*fun) (void*);
    void* arg;
    struct _green_t* next;
    struct _green_t* join; 
    void* retval; 
    int zombie;
} green_t;

/* Representation of our green_cond_var */
typedef struct _green_cond_t
{
    green_t* head; 
} green_cond_t; 

/* Representation of our green_mutex */
typedef struct _green_mutex_t
{
    volatile int taken;
    // Handle the list 
    green_t* head; 
} green_mutex_t;

/* Print the ready-queue */
void print_queue(void);

/* Initializes a green thread. */
int green_create(green_t* thread, void* (*fun)(void*), void* arg);

/* Suspends the current thread and selects a new thread for execution. */
int green_yield(void);

/* The current thread is suspended waiting for a thread to terminate. */
int green_join(green_t* thread, void** val); 

/* Initializes a green conddition variable */
void green_cond_init(green_cond_t* condv);

/* Suspend the current thread on the conition */
void green_cond_wait(green_cond_t* condv);

/* Move the first suspended threadd to the reay-queue */
void green_cond_signal(green_cond_t* condv);

/* Initialize a green mutex. */
int green_mutex_init(green_mutex_t* mutex);

/* Acquire the lock for green mutex */
int green_mutex_lock(green_mutex_t* mutex);

/* Release the lock for green mutex */
int green_mutex_unlock(green_mutex_t* mutex);

#endif