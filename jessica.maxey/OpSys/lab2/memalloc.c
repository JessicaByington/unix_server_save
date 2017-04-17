/*******************************************************************
 * Author:          Jessica Maxey
 * Filename:        memalloc.c
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
#include "memalloc.h"
#include <stdlib.h>
#include <stdio.h>

#define MINIMUM_SIZE 16
#define HIGHEST_ORDER 2048
#define OUT_OF_MEMORY 1

struct mlist * free_list;
struct mlist * busy_list;
char *memory;
int max_order;


/***************************************************************************
 *  Purpose: inits the memory for the free and busy lists, the memory for 
 *      the block, and all the structs that will be contained inside the
 *      free and busy lists
 *
 *  Precondition: needs to be called
 *
 *  Postcondition: memory will be allocated for the program to run.
 *
 * ************************************************************************/
void my_mem_init()
{
    //calc the highest order given our largest memory size + 1
    max_order  = compute_order(2048) + 1;
 
    //init's data from free and busy list 
    free_list = (struct mlist *) malloc(sizeof(struct mlist) * (max_order));
    busy_list = (struct mlist *) malloc(sizeof(struct mlist) * (max_order));

    //actally pulls out the chunk for our memory
    memory = (char *) malloc(2048);

    //inits the structs for each order in the free and busy lists
    for (int i = 0; i < max_order; i++)
    {
        InitList(&free_list[i]);
        InitList(&busy_list[i]);
    }

    //Init the first block which is all the memory
    Push(&free_list[max_order - 1], 0);
//    printf("Pushing %d to order %d\n", 0, max_order - 1);
}

/***************************************************************************
 *  Purpose: calc's the order for the value that is passed in
 *
 *  Precondition: needs a size to find the order for
 *
 *  Postcondition: a order is calc'd and sent back
 *
 * ************************************************************************/
int compute_order(int size)
{
    int curr_size = MINIMUM_SIZE;
    int order = 0;

    while(curr_size < size)
    {
        curr_size <<= 1;
        order++;
    }

    return order;
}

/***************************************************************************
 *  Purpose: to split the chucks of memory in halves so that memory can be 
 *      writen to it.
 *
 *  Precondition: order for the size of memory should have already been calc
 *
 *  Postcondition: block that is not busy will have been split so that it 
 *      can have the value added to it
 *
 * ************************************************************************/
void split(int order)
{
    //max sure it is not going out of bounds
    if(order < max_order)
    {
        if(free_list[order].num_used == 0)
            split(order + 1);

        if(free_list[order].num_used == 0) 
        {
            fprintf(stderr, "Error, out of memory, returning to my malloc\n");
            return;
        }
    
        //this is how get the block
        int alloc_block = Pop(&free_list[order]);
        //printf("Popping %d from order %d\n", alloc_block, order);

        //this is how we split the block
        Push(&free_list[order - 1], alloc_block);
        //printf("Pushing %d to order %d\n", alloc_block, order - 1);
    
        Push(&free_list[order - 1], alloc_block + (MINIMUM_SIZE <<(order - 1)));
        //printf("Pushing %d to order %d\n", alloc_block + (MINIMUM_SIZE << (order - 1)), order - 1);
    }
}

/***************************************************************************
 *  Purpose: allocates memory from the free list to the busy list given a 
 *      size that is needed.
 *
 *  Precondition: pass in a size, init should have been called
 *
 *  Postcondition: block of memory will have been allocated for the size 
 *      given
 *
 * ************************************************************************/
void * my_malloc(int size)
{
    //get the order
    int order = compute_order(size);

    //max sure it is not going out of bounds, return null if so
    if(order < max_order)
    {
        //splits the block to the smallest needed
        if(free_list[order].num_used == 0)
            split(order + 1);
    
        //check to see if we ran out of memory
        if(free_list[order].num_used == 0)
        {
            fprintf(stderr, "Error, 0ut of memory, returning NULL\n");
            return NULL;
        }
    
        //get the block to allocate
        int alloc_block = Pop(&free_list[order]);
        //printf("Popping %d from order %d\n", alloc_block, order);
        
        //adds to busy list
        Push(&busy_list[order], alloc_block);

        //return address to user
        return (memory + alloc_block);
        }

    return NULL;
}

/***************************************************************************
 *  Purpose: take the memory addess given by the user and free it if it is
 *      busy
 *
 *  Precondition: memory has already been allocated, and user has given 
 *      a address
 *
 *  Postcondition: memory will have been freed from the busy list, and 
 *      will have been coalesced together if it's buddy is free
 *
 * ************************************************************************/
void my_free(void * ptr)
{
    int offset = (char *)ptr - memory;
    int found = 0;
    int order  = 0;
    int size = 0;
    int buddy = 0;

    //printf("Free offset %d\n", offset);


    //check to see if the user has passed in a valid address
    if (!((offset) >= 0 && (offset) <= 2048))
    {
        fprintf(stderr, "Memory not valid\n");
        return;
    }
    
    //loop through to find the order that the memory resides in inside the
    //busy list
    while(found != 1)
    {
        if(order <= max_order)
        {
            found = DeleteElement(&busy_list[order], offset);
    
            //if (found == 1)
                //printf("Deleting 0x%04X from order %d\n", offset, order);

            if(found == 0)
                order++;
        }
    }
    
    //check to see that it is not the biggest free block, 
    //which would not have a buddy, and there for we won't need to check
    if(order != max_order)
    {
        //get the size
        size = MINIMUM_SIZE << order;

        //find the buddy's offset
        buddy = offset ^ size;

        //reset the var bool
        found = 0;
    
        //check to see if buddy is busy
        found = DeleteElement(&free_list[order], buddy);
    
        //if (found == 1)
            //printf("Deleting 0x%04X from order %d\n", buddy, order);
    }

    //if found == 0, means that it's buddy is busy, and we can return
    if(found != 1)
    {
        //if buddy wasn't found, then put the busy in current blocks free list
        Push(&free_list[order], offset);
        //printf("Found not true, Pushing 0x%04X from order %d\n", offset, order);
    }
    //buddy was found, now need to coalesce
    else
    {
        //combine in the smaller address level
        //into order plus 1
        //then recurse
        if (offset < buddy)
        {
            Push(&busy_list[order + 1], offset);
            //printf("Found true, Pushing offset 0x%04X from order %d\n", offset, order + 1);

            my_free(memory + offset);
        }
        else
        {
            Push(&busy_list[order + 1], buddy);
            //printf("Found true, Pushing buddy %d from order %d\n", buddy, order + 1);

            my_free(memory + buddy);
        }


    }



}

/***************************************************************************
 *  Purpose: to validate that meta data did not get corrupted
 *
 *  Precondition: init should have been called, and memory should have been
 *      allocated and or freed
 *
 *  Postcondition: will print out messages if it is corrupt, nothing will
 *      print if there was no corruption found.
 *
 * ************************************************************************/
void my_validate()
{
    //checking to see if the lower bits in each list are zero
    int bits = 0x000F;
    int size = 0;

    //check lower bits for free list
    for (int i = 0; i < max_order; i++)
    {
        for (int j = 0; j < free_list[i].num_used; j++)
        {
            if((free_list[i].elements[j] & bits) != 0)
            {
                fprintf(stderr, "Meta data corruption found\n");
                return;
            }

        }
        
        bits = (bits << 1) + 1;
    }
    
    //reset the bits
    bits = 0x000F;

    //check lower bits are vaild on busy list
    for (int i = 0; i < max_order; i++)
    {
        for (int j = 0; j < busy_list[i].num_used; j++)
        {
            if((busy_list[i].elements[j] & bits) != 0)
            {
                fprintf(stderr, "Meta data corruption found\n");
                return;
            }

        }
        
        bits = (bits << 1) + 1;
    }

    //check to make sure that the size is equal to the max size 2048
    //if less, it is corrupt, if more it is corrupt
    for (int i = 0; i < max_order; i++)
    {
        size += free_list[i].num_used * (MINIMUM_SIZE << i);
        size += busy_list[i].num_used * (MINIMUM_SIZE << i);
    }


    if(size != 2048)
    {
        fprintf(stderr, "Meta data corruption found\n");
        return;
    }

}

/***************************************************************************
 *  Purpose: print out meta data for all blocks
 *
 *  Precondition: init must have been called
 *
 *  Postcondition: all meta data for all blocks will be printed out in a
 *      readable fashion.
 *
 * ************************************************************************/
void my_print_mem()
{
    fprintf(stdout, "\n");
    
    //print out for free list
    for (int i = 0; i < max_order; i++)
    {
        fprintf(stdout, "free 0x%04X = ", MINIMUM_SIZE << i);

        for (int j = 0; j < free_list[i].num_used; j++)
        {
            fprintf(stdout, "0x%04X", free_list[i].elements[j]);

            if (j < free_list[i].num_used - 1)
                fprintf(stdout, ", ");
        }

        fprintf(stdout, "\n");
    } 

    fprintf(stdout, "\n");

    //print out for busy list
    for (int i = 0; i < max_order; i++)
    {
        fprintf(stdout, "busy 0x%04X = ", MINIMUM_SIZE << i);

        for (int j = 0; j < busy_list[i].num_used; j++)
        {
            fprintf(stdout, "0x%04X", busy_list[i].elements[j]);
            
            if (j < busy_list[i].num_used - 1)
                fprintf(stdout, ", ");
        }

        fprintf(stdout, "\n");
    }

    fprintf(stdout, "\n");
    
}
