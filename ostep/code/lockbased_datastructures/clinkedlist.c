#include <pthread.h>

typedef struct __node_t
{
    int key; 
    struct __node_t* next; 
} node_t; 

typedef struct __list_t
{
    node_t* head; 
    pthread_mutex_t lock;
} list_t; 

void list_init(list_t* l)
{
    l->head = NULL; 
    pthread_mutex_init(&l->lock, NULL);
}

int list_insert(list_t* l, int key)
{
    pthread_mutex_lock(&l->lock);
    node_t* new = malloc(sizeof(node_t));
    if (new == NULL)
    {
        perror("malloc");
        pthread_mutex_unlock(&l->lock);
        return -1; 
    }

    new->key = key;  
    new->next = l->head; 
    l->head = new; 
    pthread_mutex_unlock(&l->lock);
    return 0; 
}

int list_lookup(list_t* l, int key)
{
    pthread_mutex_lock(&l->lock);
    node_t* curr = l->head; 
    while (curr)
    {
        if (curr->key == key)
        {
            pthread_mutex_unlock(&l->lock);
            return 0; 
        }
        curr = curr->next;
    }
    pthread_mutex_unlock(&l->lock); 
    return -1; 
}