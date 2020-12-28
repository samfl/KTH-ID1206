#include "cqueue.h"
#include "green.h"

void queue_init(queue_t* q)
{
    node_t* tmp = malloc(sizeof(node_t));
    tmp->next = NULL; 
    q->head = q->tail = tmp; 
    pthread_mutex_init(&q->head_lock, NULL); 
    pthread_mutex_init(&q->tail_lock, NULL); 
}

void queue_enqueue(queue_t* q, green_t* thread)
{
    node_t* tmp = malloc(sizeof(node_t));
    assert(tmp != NULL);
    tmp->thread = *thread;
    tmp->next = NULL; 

    pthread_mutex_lock(&q->tail_lock);
    q->tail->next = tmp; 
    q->tail = tmp; 
    pthread_mutex_unlock(&q->tail_lock);
}

int queue_dequeue(queue_t* q, green_t* thread)
{
    pthread_mutex_lock(&q->head_lock);
    node_t* tmp = q->head;
    node_t* new_head = tmp->next; 
    if (new_head == NULL)
    {
        pthread_mutex_unlock(&q->head_lock);
        return -1; 
    }
    *thread = tmp->thread;
    q->head = new_head; 
    pthread_mutex_unlock(&q->head_lock);
    free(tmp);
    return 0; 
}