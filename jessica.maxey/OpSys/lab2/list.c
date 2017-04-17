/*******************************************************************
 * Author:          Jessica Maxey
 * Filename:        list.c
 * Date Created:    4/8/16
 * Modificatons:    
 * *****************************************************************/
/*******************************************************************
 * Lab/Assignment: Lab 2 - Memory Allocation
 *
 *  Overview: 
 *      This program is a implementation of a Buddy System memory 
 *      allocator.     
 *
 *  Input:
 *      Input will be the size of the block of memory that is wanted,
 *      or a memory address that is to be freed.
 *
 *  Output:
 *      Out will be the address of the block of memory that was just
 *      allocated, error messages if memory is not available or
 *      vaild, or the printed out view of what memory is being used
 *      in the free list or buddy list
 *
 *******************************************************************/
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/***************************************************************************
 *  Purpose: To init memory, and data for the variables stored in the list
 *
 *  Precondition: n/a
 *      
 *  Postcondition: memory has been allocated for the array of elements
 *
 * ************************************************************************/
void InitList(struct mlist * list)
{
    list->elements = (int*) malloc((2048/16) + 1);
    list->num_used = 0;
}

/***************************************************************************
 *  Purpose: To push data onto the end of the elements array
 *
 *  Precondition: list needs to have been created, and an value needs to be 
 *      passed in
 *
 *  Postcondition: the value has been added to the end of the array
 *
 * ************************************************************************/
void Push(struct mlist * list, int value)
{
    if(list->num_used <= (2048/16))
    {
        list->elements[list->num_used] = value;
        list->num_used++;
    }
    else
        fprintf(stderr, "Trying to add to a full list\n");
}


/***************************************************************************
 *  Purpose: to remove the last element in the array
 *
 *  Precondition: array needs to have something in it
 *
 *  Postcondition: the las element will have been removed
 *
 * ************************************************************************/
int Pop(struct mlist * list)
{
    if(list->num_used != 0)
    {
        list->num_used--;
        int value = list->elements[list->num_used];
        return value;
    }
    
    fprintf(stderr, "Trying to remove from empty list\n");
    return -1;
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
int DeleteElement(struct mlist * list, int value)
{
    for (int i = 0; i < list->num_used; i++)
    {
        if(list->elements[i] == value)
        {
            memmove(&list->elements[i], &list->elements[i+1], sizeof(int) * (list->num_used - i - 1));
            list->num_used--;
            return 1;
        }
    }

    return 0;

}

