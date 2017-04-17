/*************************************************************
* Author:        Philip Howard
* Email:         phil.howard@oit.edu
* Filename:      list.c
* Date Created:  2016-04-27
* Edited By:     Jessica Maxey
**************************************************************/

#include <stdlib.h>
#include <assert.h>

#include "hhlist.h"

/********************************************************************** 
* Purpose: Initialize a list
*
* Precondition: 
*   None
*
* Postcondition: 
*   A pointer to a properly initialized list is returned
*
************************************************************************/
list_t *list_init()
{
    list_t *list = (list_t *)malloc(sizeof(list_t));
    assert(list != NULL);

    list->count = 0;
    list->first = NULL;
    list->last = NULL;
    pthread_mutex_init(&list->m_lock, NULL);
            

    return list;
}

/********************************************************************** 
* Purpose: Add an item to the list in sorted order
*
* Precondition: 
*   list points to a well formed and sorted list
*
* Postcondition: 
*   A new item is added to the list in sorted order
*
************************************************************************/
void list_sorted_insert(list_t *list, int value)
{
    pthread_mutex_lock(&list->m_lock);
    
    list_item_t *item;
    list_item_t *prev;

    assert(list != NULL);

    list_item_t *new_item = (list_item_t *)malloc(sizeof(list_item_t));
    assert(new_item != NULL);
    pthread_mutex_init(&new_item->m_lock, NULL);

    new_item->value = value;
    if (list->first == NULL || value < list->first->value) 
    {
        if (list->first != NULL)
           pthread_mutex_lock(&list->first->m_lock);
        
            // special case inserting at beginning of list
        new_item->next = list->first;
        list->first = new_item;

        if (list->last == NULL) list->last = new_item;

        if(list->first->next != NULL)
            pthread_mutex_unlock(&list->first->next->m_lock);

        pthread_mutex_unlock(&list->m_lock);
    }
    else
    {
        item = list->first;
    
        pthread_mutex_lock(&item->m_lock);
        pthread_mutex_unlock(&list->m_lock);

        while (item->next != NULL && item->next->value < value)
        {
            pthread_mutex_lock(&item->next->m_lock);
        
            prev = item;
            item = item->next;

            pthread_mutex_unlock(&prev->m_lock);
        }

        if(item->next != NULL)
            pthread_mutex_lock(&item->next->m_lock);

        //sched_yield();
        new_item->next = item->next;
        item->next = new_item;

        if (item->next == NULL) list->last = new_item;

        if(new_item->next != NULL)
            pthread_mutex_unlock(&new_item->next->m_lock);

        pthread_mutex_unlock(&item->m_lock);
    }

    pthread_mutex_lock(&list->m_lock);
    list->count++;
    pthread_mutex_unlock(&list->m_lock);
    
}
/********************************************************************** 
* Purpose: Adds a new element to the front of the list
*
* Precondition: 
*   list points to a well formed list
*
* Postcondition: 
*   A new item is added to the front of the list
*
************************************************************************/
void list_push(list_t *list, int value)
{
    pthread_mutex_lock(&list->m_lock);

//    list_item_t *item;

    assert(list != NULL);

    list_item_t *new_item = (list_item_t *)malloc(sizeof(list_item_t));
    assert(new_item != NULL);
    pthread_mutex_init(&new_item->m_lock, NULL);

    pthread_mutex_lock(&list->first->m_lock);

    new_item->value = value;
    new_item->next = NULL;

    // insert at beginning
    new_item->next = list->first;
    list->first = new_item;

    if (list->last == NULL) list->last = new_item;

    list->count++;

    pthread_mutex_unlock(&new_item->next->m_lock);
    pthread_mutex_unlock(&list->m_lock);

}
/********************************************************************** 
* Purpose: place an item at the end of the list
*
* Precondition: 
*   list points to a well formed list
*
* Postcondition: 
*   A new item is added to the end of the list
*
************************************************************************/
void list_push_end(list_t *list, int value)
{
    pthread_mutex_lock(&list->m_lock);

    assert(list != NULL);

    list_item_t *item = (list_item_t *)malloc(sizeof(list_item_t));
    list_item_t *prev;

    assert(item != NULL);
    pthread_mutex_init(&item->m_lock, NULL);

    item->value = value;
    item->next = NULL;

    pthread_mutex_lock(&list->last->m_lock);

    if (list->last == NULL)
    {
        list->first = item;
        list->last = item;
    }
    else
    {
        prev = list->last;
        list->last->next = item;
        list->last = item;
    }

    pthread_mutex_unlock(&prev->m_lock);

    list->count++;
    
    pthread_mutex_unlock(&list->m_lock);

}
/********************************************************************** 
* Purpose: Remove an item from the front of the list
*
* Precondition: 
*   list points to a well formed list
*
* Postcondition: 
*   The item at the front of the list is returned and removed.
*   If the list is empty, zero is returned
*
************************************************************************/
int list_pop(list_t *list)
{
    pthread_mutex_lock(&list->m_lock);

    int value;
    list_item_t *item;

    if (list->first == NULL) 
    {
        pthread_mutex_unlock(&list->m_lock);
        return 0;
    }

    pthread_mutex_lock(&list->first->m_lock);

    item = list->first;
    
    value = item->value;
    list->first = item->next;

    if (list->first == NULL) list->last = NULL;
        list->count--;

    free(item);

    pthread_mutex_unlock(&list->m_lock);

    return value;
}
