/*******************************************************************
 * Author:          Jessica Maxey
 * Filename:        list.h
 * Date Created:    4/8/16
 * Modificatons:    
 * *****************************************************************/
#include <stdio.h>

struct mlist
{
    int *elements;
    int num_used;
};

void InitList(struct mlist *);
void Push(struct mlist *, int value);
int Pop(struct mlist *); 
int DeleteElement(struct mlist *, int element);
