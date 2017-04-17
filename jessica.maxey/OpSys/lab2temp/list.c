#include "list.h"
#include <stdio.h>
#include <stdlib.h>

struct mlist * InitList()
{
    struct mlist * list;
    list = (struct mlist *)  malloc(sizeof(struct mlist));
    list->elements = (int*) malloc((2048/16) + 1);
    list->num_used = 0;

    return list;
}

void Add(int element)
{
}

void Delete()
{
}

void DeleteElement(int element)
{
}

int GetNumUsed(struct mlist list)
{
   return list.num_used;
}

void IncreamentNumUsed(struct mlist free_list)
{
    free_list.num_used++;
}

void DecreamentNumUsed(struct mlist busy_list)
{
    busy_list.num_used--;
}
