#include <stdio.h>

struct mlist
{
    int * elements;
    int num_used;
};

struct mlist * InitList();
void Add(int element);
void Delete(); 
void DeleteElement(int element);
int GetNumUsed(struct mlist); 
void IncreamentNumUsed(struct mlist);
void DecreamentNumUsed(struct mlist);
