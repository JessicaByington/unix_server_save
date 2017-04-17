//********************************************
// Thread-safe queue implementation
//
// Author: Phil Howard
// Email:  phil.howard@oit.edu
//

#include <assert.h>
#include <stdlib.h>
#include <pthread.h>

#include "queue.h"

// Items that get queued
typedef struct item_qs
{
    void *data;                 // location for user's data
    struct item_qs *next;
} item_qt;

// Structure for actual queue
typedef struct 
{
    item_qt *head;
    item_qt *tail;
    int is_open;
    pthread_mutex_t lock;
    pthread_cond_t  cond;
} q_type;

//********************************
queue_t queue_create()
{
    q_type *queue;

    queue = (q_type *)malloc(sizeof(q_type));
    assert(queue != NULL);

    pthread_mutex_init(&queue->lock, NULL);
    pthread_cond_init(&queue->cond, NULL);

    pthread_mutex_lock(&queue->lock);

    queue->head = NULL;
    queue->tail = NULL;
    queue->is_open = 1;

    pthread_mutex_unlock(&queue->lock);

    return queue;
}
//********************************
void queue_destroy(queue_t q)
{
    free(q);
}
//********************************
void queue_insert(queue_t q, void *data)
{
    assert(q != NULL);
    q_type *queue = (q_type *)q;

    item_qt *item = (item_qt *)malloc(sizeof(item_qt));
    assert(item != NULL);

    pthread_mutex_lock(&queue->lock);
    item->data = data;
    item->next = NULL;
    if (queue->tail != NULL) queue->tail->next = item;
    queue->tail = item;
    if (queue->head == NULL) queue->head = item;
    pthread_mutex_unlock(&queue->lock);
    pthread_cond_signal(&queue->cond);
}
//********************************
void *queue_remove(queue_t q)
{
    item_qt *item;
    char *data;

    assert(q != NULL);
    q_type *queue = (q_type *)q;

    pthread_mutex_lock(&queue->lock);
    while (queue->head == NULL && queue->is_open) 
    {
        pthread_cond_wait(&queue->cond, &queue->lock);
    }

    if (queue->head == NULL)
    {
        if (!queue->is_open) pthread_cond_broadcast(&queue->cond);
        pthread_mutex_unlock(&queue->lock);
        return NULL;
    }

    item = queue->head;
    queue->head = item->next;
    if (queue->head == NULL) queue->tail = NULL;

    if (!queue->is_open) pthread_cond_broadcast(&queue->cond);
    pthread_mutex_unlock(&queue->lock);

    data = item->data;

    free(item);

    return data;
}
//**********************************
void queue_mark_closed(queue_t q)
{
    assert(q != NULL);
    q_type *queue = (q_type *)q;

    pthread_mutex_lock(&queue->lock);
    queue->is_open = 0;
    pthread_mutex_unlock(&queue->lock);
    pthread_cond_broadcast(&queue->cond);
}
//**********************************
int queue_is_open(queue_t q)
{
    int is_closed;

    if (q == NULL) return 0;
    q_type *queue = (q_type *)q;

    pthread_mutex_lock(&queue->lock);
    is_closed = (!queue->is_open && queue->head == NULL) ;
    pthread_mutex_unlock(&queue->lock);

    return !is_closed;
}
