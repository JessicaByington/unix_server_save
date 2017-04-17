#include "list.h"
#include "memalloc.h"
#include <stdlib.h>
#include <stdio.h>

#define MINIMUM_SIZE = 16;
#define HIGHEST_ORDER = 2048;

int main (int argc, char * argv[])
{

    return 0;
}

void my_mem_init()
{
   struct mlist * free_list;
   array_size = compute_order(2048) + 1;
   free_list = (struct mlist *) malloc(array_size);
   busy_list = (struct mlist *) malloc(array_size);

   for (int i = 0; i < array_size; i++)
   {
       free_list[i] = InitList();
       busy_list[i] = InitList();
   }
}

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

void * my_malloc(int size)
{
}

void my_free(void * ptr)
{
}

void my_validate()
{
}

void my_print_mem()
{
}
