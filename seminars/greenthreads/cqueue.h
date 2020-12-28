#ifndef _CQUEUE_H
#define _CQUEUE_H

#include <pthread.h>
#include "green.h"

typedef struct __node_t
{
    green_t thread; 
    struct __node_t* next;
} node_t; 

typedef struct __queue_t
{
    node_t* head; 
    node_t* tail; 
    pthread_mutex_t head_lock; 
    pthread_mutex_t tail_lock; 
} queue_t; 

void queue_init(queue_t* q);
void queue_enqueue(queue_t* q, green_t val);
void queue_dequeue(queue_t* q, green_t* val);

#endif