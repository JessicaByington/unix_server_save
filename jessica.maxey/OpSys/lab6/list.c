/*************************************************************
* Author:        Philip Howard
* Email:         phil.howard@oit.edu
* Filename:      list.c
* Date Created:  2016-04-27
* Edited By:     Jessica Maxey
**************************************************************/

#include <stdlib.h>
#include <assert.h>

#include "list.h"

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
* Purpose: Adds a new element to the front of the list
*
* Precondition: 
*   list points to a well formed list
*
* Postcondition: 
*   A new item is added to the front of the list
*
************************************************************************/
void list_push(list_t *list, thread value)
{
    pthread_mutex_lock(&list->m_lock);

    //list_item_t *item;

    assert(list != NULL);

    list_item_t *new_item = (list_item_t *)malloc(sizeof(list_item_t));
    assert(new_item != NULL);

    new_item->value = value;
    new_item->next = NULL;

    // insert at beginning
    new_item->next = list->first;
    list->first = new_item;

    if (list->last == NULL) list->last = new_item;

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
int SearchElement(list_t * list, thread value)
{
    list_item_t * t_ptr = list->first;

    while(t_ptr != NULL)
    {
        if(t_ptr->value == value)
        {
            return 1;
        }

        t_ptr = t_ptr->next;
    }


    return 0;

}

/***************************************************************************
 *  Purpose: to remove a value from the list at it's location and then move
 *      data down to fill in the empty space
 *
 *  Precondition: list needs to have a value in it
 *
 *  Postcondition: value will have been removed, and the other data will 
 *      have been moved to fill the void.
 *
 * ************************************************************************/
int DeleteElement(list_t * list, thread value)
{
    list_item_t * t_ptr = list->first;
    list_item_t * prev = NULL;

    while(t_ptr != NULL)
    {
        if(t_ptr->value == value)
        {
            if(prev == NULL)
                list->first = t_ptr->next;
            else
                prev->next = t_ptr->next;
            free(t_ptr);
            return 1;
        }

        prev = t_ptr;
        t_ptr = t_ptr->next;
    }


    return 0;

}
