/*******************************************************************
 * Author:          Jessica Maxey
 * Filename:        memalloc.h
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
#include <stdlib.h>

void my_mem_init();
int compute_order(int size);
void split(int order);
void * my_malloc(int size);
void my_free(void *ptr);
void my_validate();
void my_print_mem();
extern struct mlist * free_list;
extern struct mlist * busy_list;
extern char * memory;
extern int max_order;
