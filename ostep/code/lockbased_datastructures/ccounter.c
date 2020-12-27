/***
 * 
 * A Counter Without Locks
 * Figure 29.1 p.338
 * 
***/
#include <pthread.h>

typedef struct __counter_t
{
    int value; 
} counter_t;

void init(counter_t* c)
{
    c->value = 0; 
}

void increment(counter_t* c)
{
    c->value++; 
}

void decrement(counter_t* c)
{
    c->value--; 
}

int get(counter_t* c)
{
    return c->value; 
}

/***
 * 
 * A Counter With Locks
 * Figure 29.2 p.338
 * 
***/

typedef struct __counter_lock_t
{
    int value; 
    pthread_mutex_t lock; 
} counter_lock_t;

void init(counter_lock_t* c)
{
    c->value = 0; 
    pthread_mutex_init(&c->lock, NULL);
}

void increment(counter_lock_t* c)
{
    pthread_mutex_lock(&c->lock);
    c->value++; 
    pthread_mutex_unlock(&c->lock);
}

void decrement(counter_lock_t* c)
{
    pthread_mutex_lock(&c->lock);
    c->value--; 
    pthread_mutex_unlock(&c->lock);
}

int get(counter_lock_t* c)
{
    pthread_mutex_lock(&c->lock);
    int rc = c->value; 
    pthread_mutex_unlock(&c->lock);
    return rc; 
}