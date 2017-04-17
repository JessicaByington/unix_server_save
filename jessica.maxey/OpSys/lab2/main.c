/*******************************************************************
 * Author:          Jessica Maxey
 * Filename:        main.c
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

#include <stdio.h>
#include <stdlib.h>

/***************************************************************************
 *  Purpose: to drive the program 
 *
 *  Precondition: what values to allocate, free, print or validate
 *
 *  Postcondition: memory will be freed allocated, printed or vaildated
 *
 * ************************************************************************/
int main(int argc, char * argv[])
{
    my_mem_init();
    my_print_mem();
    
    void * address_1 = my_malloc(5);
    my_print_mem();
    printf("Value returned from my malloc: %p\n", address_1);
    
    void * address_2 = my_malloc(5);
    my_print_mem();
    printf("Value returned from my malloc: %p\n", address_2);

    void * address_3 = my_malloc(5);
    my_print_mem();
    printf("Value returned from my malloc: %p\n", address_3);
    
    my_free(address_1);
    my_print_mem();
    my_validate();

    my_free(address_2);
    my_print_mem();
    my_validate();

    void * address_4 = my_malloc(1024);
    my_print_mem();
    printf("Value returned from my malloc: %p\n", address_4);
    
    void * address_5 = my_malloc(50);
    my_print_mem();
    printf("Value returned from my malloc: %p\n", address_5);
    
    void * address_6 = my_malloc(140);
    my_print_mem();
    printf("Value returned from my malloc: %p\n", address_6);
    
    void * address_7 = my_malloc(200);
    my_print_mem();
    printf("Value returned from my malloc: %p\n", address_7);
    
    my_free(address_3);
    my_validate();

    my_free(address_4);
    my_validate();
    
    my_free(address_5);
    my_validate();
    
    my_free(address_6);
    my_validate();
    
    my_free(address_7);
    my_validate();
   
    my_print_mem(); 
   
    void * address_8 = my_malloc(1024);
    my_print_mem();
    printf("Value returned from my malloc: %p\n", address_8);

    my_validate();

    void * address_9 = my_malloc(1024);
    my_print_mem();
    printf("Value returned from my malloc: %p\n", address_9);

    my_validate();


    void * address_10 = my_malloc(1024);
    my_print_mem();
    printf("Value returned from my malloc: %p\n", address_10);

    my_validate();
    
    return 0;
}
