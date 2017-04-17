/*************************************************************
 * Author:        Philip Howard
 * Filename:      ready_queue.c
 * Date Created:  5/4/2016
 * Modifications:
 * Edited by:     Jessica Maxey 
 **************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>

#include "ready_queue.h"

// The actual definition of the queue type.
// NOTE: This is pseudo-code. You will have to make a real definition
typedef struct queue_item_s
{
    thread value;
    struct queue_item_s *next;
} my_queue_item_t;

typedef struct queue_s
{
    struct queue_item_s *first;
    struct queue_item_s *last;
    pthread_mutex_t lock;
    int closed;
    pthread_cond_t queue_cv;
} my_queue_t;


/********************************************************************** 
 * Purpose: This function initializes a queue. 
 * It returns an opaque pointer to the queue. The queue must be destroyed when
 * no longer needed by calling Q_Destroy()
 *
 * Precondition: 
 *     None
 *
 * Postcondition: 
 *      Queue has been created.
 *      Returns NULL on failure
 *
 ************************************************************************/
queue_t Q_Init()
{
    my_queue_t *queue = malloc(sizeof(my_queue_t));
    assert(queue != NULL);

    
    if(pthread_mutex_init(&queue->lock, NULL) != 0)
    {
        fprintf(stderr, "Failed to init pthread_mutexi for queue_lock\n");
        return NULL;
    }

    
    if(pthread_cond_init(&queue->queue_cv, NULL) != 0)
    {
        fprintf(stderr, "Failed to init pthread_cond for &queue->queue_cv\n");
        return NULL;
    }
    
    pthread_mutex_lock(&queue->lock);

    queue->closed = 0;
    queue->first = NULL;
    queue->last = NULL;

    pthread_mutex_unlock(&queue->lock);

    return (queue_t)queue;
}

/********************************************************************** 
 * Purpose: This function cleans up any memory occupied by the queue. 
 * It should only be called when the queue is no longer needed, and is no
 * longer being accessed by any other threads.
 *
 * Precondition: 
 *     The queue is a valid queue that has been closed and emptied.
 *     No other threads are currently accessing the queue, and none will in
 *     the future.
 *
 * Postcondition: 
 *      Queue has been destroyed; all memory has been reclaimed.
 *      Returns zero on success and non-zero on failure
 *
 ************************************************************************/
int Q_Destroy(queue_t queue)
{
    my_queue_t * q = (my_queue_t *)queue;
    my_queue_item_t * prev; 

    //check to see if closed, if not return -1
    if(q->closed == 1)
    {
        if(pthread_cond_destroy(&q->queue_cv) != 0)
        {
            fprintf(stderr, "Failed to destroy q->queue_cv\n");
            return -1;
        }

        if(pthread_mutex_destroy(&q->lock) != 0)
        {
            fprintf(stderr, "Failed to destroy q->lock\n");
            return -1;
        }

        //check to see if empty, if not empty
        while (q->first != NULL)
        {
            prev = q->first;
            q->first = q->first->next;

            free(prev);
        }

        free(q);
    

        return 0;
    }


    return -1;
}

/********************************************************************** 
 * Purpose: This markes the queue as closed. Dequeue operations are allowed
 * after a queue is marked as closed, but no further enqueue operations should
 * be performed.
 *
 * Precondition: 
 *     Queue is a valid queue.
 *
 * Postcondition: 
 *      Queue has been marked as closed.
 *      Returns zero on success, non-zero on failure
 ************************************************************************/
int Q_Close(queue_t q)
{
    assert(q != NULL);
    my_queue_t *queue = (my_queue_t *)q;
    
    pthread_mutex_lock(&queue->lock);
    queue->closed = 1;
    pthread_mutex_unlock(&queue->lock);

    if(pthread_cond_broadcast(&queue->queue_cv) != 0)
    {
        fprintf(stderr, "Failed to send pthread_cond_broadcast for &queue->queue_cv\n");
        return -1;
    }

    return 0;
}

/********************************************************************** 
 * Purpose: Places a new element into the queue.
 *
 * Precondition: 
 *     Queue is a valid queue that has not been marked as closed.
 *
 * Postcondition: 
 *      Queue contains one additional element
 *      Returns zero on success, non-zero on failure
 ************************************************************************/
int Q_Enqueue(queue_t queue, thread buffer)
{
    my_queue_t *q = (my_queue_t *)queue;

    if(pthread_mutex_lock(&q->lock) != 0)
    {
        fprintf(stderr, "Failed to lock &q->lock\n");
        return -1;
    }

    assert(q != NULL);

    my_queue_item_t * item = (my_queue_item_t *) malloc(sizeof(my_queue_item_t));
    assert(item != NULL);

    item->value = buffer;
    item->next = NULL;

    if(q->last != NULL) q->last->next = item;
    q->last = item;
    if(q->first == NULL) q->first = item;

    if(pthread_cond_signal(&q->queue_cv) != 0)
    {
        fprintf(stderr, "Failed to signal &q->queue_cv\n");
        return -1;
    }

    if(pthread_mutex_unlock(&q->lock) != 0)
    {
        fprintf(stderr, "Failed to unlock &q->lock\n");
        return -1;
    }

    return 1;
}

/********************************************************************** 
 * Purpose: Removes an element from the queue
 *
 * Precondition: 
 *     Queue is a valid queue
 *
 * Postcondition: 
 *      If the queue was not empty, it contains one less element
 *      Returns a pointer to the string stored in the queue.
 *      Returns NULL if the queue is empty.
 *      NOTE: This behavior will be changed as part of this lab
 ************************************************************************/
thread Q_Dequeue(queue_t queue)
{
    my_queue_t *q = (my_queue_t *)queue;
    
    if(pthread_mutex_lock(&q->lock) != 0)
    {
        fprintf(stderr, "Failed to lock &q->lock\n");
        return NULL;
    }

    
    thread * value;
    my_queue_item_t * item;

    if(q->first == NULL)
    {
        if(pthread_mutex_unlock(&q->lock) != 0)
        {
            fprintf(stderr, "Failed to unlock &q->unlock\n");
            return NULL;
        }

        return NULL;
    }

    item = q->first;

    while(Q_Is_Closed(queue) != 0 && (item->value == NULL))
        if(pthread_cond_wait(&q->queue_cv, &q->lock) != 0)
        {
            fprintf(stderr, "Failed to wait for pthread_cond_wait on &q->queue_cv\n");
            return NULL;
        }
    
    item = q->first;
    q->first = item->next;
    
    if(q->first == NULL)
        q->last = NULL;

    if(q->closed)
        pthread_cond_broadcast(&q->queue_cv);

    if(pthread_mutex_unlock(&q->lock) != 0)
    {
        fprintf(stderr, "Failed to unlock &q->lock\n");
        return NULL;
    }

    value = item->value;

    free(item);

    return value;
}

/********************************************************************** 
 * Purpose: Indicates whether the queue is open
 *
 * Precondition: 
 *     Queue is a valid queue
 *
 * Postcondition: 
 *      Returns zero if either the queue has not been marked as close OR
 *                             the queue is not empty
 *      Returns non-zero of BOTH: the queue has been marked as closed AND
 *                                the queue is empty
 ************************************************************************/
int Q_Is_Closed(queue_t queue)
{
    my_queue_t *q = (my_queue_t *)queue;

    pthread_mutex_lock(&q->lock);

    if(q == NULL)
    {
        pthread_mutex_unlock(&q->lock);
        return 0;
    }
    else if(q->closed == 1 && q->first == NULL)
    {
        pthread_mutex_unlock(&q->lock);
        return 0;
    }

    pthread_mutex_unlock(&q->lock);
    return 1;
}

