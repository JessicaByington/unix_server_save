/*************************************************************
 * Author:        Philip Howard
 * Filename:      prod_cons.h
 * Date Created:  5/4/2016
 * Modifications:
 * Edited by:     Jessica Maxey 
 **************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>

#include "prod_cons.h"

// The actual definition of the queue type.
// NOTE: This is pseudo-code. You will have to make a real definition
typedef struct queue_item_s
{
    char * value;
    struct queue_item_s *next;
} my_queue_item_t;

typedef struct queue_s
{
    //int item1;
    //int item2;
    //int closed;
    int count;
    struct queue_item_s *first;
    struct queue_item_s *last;
    pthread_mutex_t lock;
    int closed;
    pthread_mutex_t queue_mutex;
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

    queue->closed = 0;
    
    queue->first = NULL;
    queue->last = NULL;
    
    if(pthread_mutex_init(&queue->lock, NULL) != 0)
    {
        fprintf(stderr, "Failed to init pthread_mutexi for queue_lock\n");
        return NULL;
    }

    if(pthread_mutex_init(&queue->queue_mutex, NULL) != 0)
    {
        fprintf(stderr, "Failed to init pthread_mutex for &queue->queue_mutex\n");
        return NULL;
    }
    
    if(pthread_cond_init(&queue->queue_cv, NULL) != 0)
    {
        fprintf(stderr, "Failed to init pthread_cond for &queue->queue_cv\n");
        return NULL;
    }
    
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

        if(pthread_mutex_destroy(&q->queue_mutex) != 0)
        {
            fprintf(stderr, "Failed to destroy q->queue_mutex\n");
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
    my_queue_t *queue = (my_queue_t *)q;
    queue->closed = 1;

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
int Q_Enqueue(queue_t queue, char *buffer)
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

    if(q->last == NULL)
    {
        q->first = item;
        q->last = item;
    }
    else
    {
        q->last->next = item;
        q->last = item;
    }

    q->count++;


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
char *Q_Dequeue(queue_t queue)
{
    my_queue_t *q = (my_queue_t *)queue;
    
    if(pthread_mutex_lock(&q->lock) != 0)
    {
        fprintf(stderr, "Failed to lock &q->lock\n");
        return NULL;
    }

    
    char * value;
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
        if(pthread_cond_wait(&q->queue_cv, &q->queue_mutex) != 0)
        {
            fprintf(stderr, "Failed to wait for pthread_cond_wait on &q->queue_cv\n");
            return NULL;
        }
    
    value = item->value;
    q->first = item->next;

    if(q->first == NULL) q->last = NULL;
    q->count--;

    free(item);

    if(pthread_mutex_unlock(&q->lock) != 0)
    {
        fprintf(stderr, "Failed to unlock &q->lock\n");
        return NULL;
    }



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

    if(q == NULL)
        return 0;
    else if(q->closed == 1 && q->first == NULL)
        return 0;

    return 1;
}

/********************************************************************** 
 * Purpose: Acts as a producer that reads from a file and places the data
 * from the file into the queue one line at a time.
 *
 * Precondition: 
 *     Queue is a valid queue
 *     filename is a valid filename for which the process has read access
 *
 * Postcondition: 
 *      The file has been read and the lines are places into the queue
 ************************************************************************/
void *producer(queue_t queue, char *filename)
{
    FILE *fp;

    fp = fopen(filename, "r");

    if(fp == NULL)
    {
        fprintf(stderr, "Error opening file\n");
        return (void*)-1;
    }
    
    while(!feof(fp))
    {
        char * buff = malloc (256);
        
        if(fgets(buff, 256, fp) != NULL)
        {
            //fprintf(stderr, "buffer %s\n", buff);

            //check stuff
            buff[strcspn(buff, "\r\n")] = 0;

            //fprintf(stdout, "after strcspn %s\n", buff);
            int i;
            for (i = 0; i < strlen(buff); i++)
            {
                if(buff[i] != ' ')
                    break;
            }

            if(i == strlen(buff))
            {
                free(buff);
            }
            else
            {
                Q_Enqueue(queue, buff);
            }
        }
        else
            free(buff);
    }
    

    if(fclose(fp) != 0)
    {
        fprintf(stderr, "Failed to close file\n");
        return (void*)-1;
    }

    return NULL;
}

/********************************************************************** 
 * Purpose: Acts as a consumer that reads from a queue and writes the lines 
 * pulled from the queue to stdout.
 *
 *
 * Precondition: 
 *     Queue is a valid queue
 *
 * Postcondition: 
 *      The file has been closed and emptied.
 ************************************************************************/
void *consumer(queue_t queue)
{
    
    while(Q_Is_Closed(queue) != 0)
    {
        char * output = Q_Dequeue(queue);        

        if (output != NULL) 
            fprintf(stdout, "%ld %s\n", pthread_self(), output);
       
        free(output);
    }
    
    return NULL;
}

