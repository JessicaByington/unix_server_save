/*************************************************************
* Author:        Philip Howard
* Email:         phil.howard@oit.edu
* Filename:      memory.c
* Date Created:  2016-04-26
* Edit By:       Jessica Maxey
**************************************************************/
#include <pthread.h>

typedef void * thread;

//*******************************************************
// Struct for elements in a singly linked list
typedef struct list_item_s
{
    thread value;               // value stored in list element
    struct list_item_s *next;   // pointer to next list element
} list_item_t;

//*******************************************************
// struct for a list made up of list_item_t elements
typedef struct list_s
{
    int count;                  // number of items in the list
    struct list_item_s *first;  // pointer to first element in list
    struct list_item_s *last;   // pointer to last element in list
    pthread_mutex_t m_lock;     // lock
} list_t;

//*******************************************************
// Initialize a list and return a pointer to it
list_t *list_init();


//*******************************************************
// Add a new item to the beginning of a list
//
// list: pointer to the list
// value: value to be added to the list
void list_push(list_t *list, thread value);

//*******************************************************
// Search the list for the value that was passed int
//
// returns a 1 if found, 0 if not found.
int SearchElement(list_t * list, thread value);

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
int DeleteElement(list_t * list, thread value);
