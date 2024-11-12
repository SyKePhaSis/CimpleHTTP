#ifndef _MEMMORY_TABLE
#define _MEMMORY_TABLE

#include <stdlib.h>

typedef struct MemT
{
    void **addresses;
    size_t *sizes;
    size_t count;
} MemT;

void insertToTable(void *address, size_t size);
void updateTable(void *address, size_t size);
void removeFromTable(void *address);
void printMemTable(void);

#endif