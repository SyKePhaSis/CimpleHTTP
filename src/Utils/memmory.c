#include "Utils/memmory.h"
#include "Utils/logger.h"
#include "Utils/errors.h"
#include "Utils/memmoryTable.h"
#include <stdlib.h>
#include <stdarg.h>

size_t allocations = 0;

void *allocate(size_t size)
{
    void *space = malloc(size);
    if (space == NULL)
    {
        ThrowMemError(__FILE__, __func__, __LINE__);
        exit(-1);
    }
    insertToTable(space, size);
    allocations++;
    return space;
}

void mass_deallocation(int c, ...)
{
    va_list vlist;
    va_start(vlist, c);
    for (int i = 0; i < c; i++)
    {
        void *block = va_arg(vlist, void *);
        deallocate(block);
    }
    va_end(vlist);
}

void deallocate(void *block)
{
    if (block != NULL)
    {
        removeFromTable(block);
        free(block);
        block = NULL;
        allocations--;
    }
    else
    {
        ThrowMemError(__FILE__, __func__, __LINE__);
    }
}

void *reallocate(void *block, size_t size)
{
    removeFromTable(block);
    void *space = realloc(block, size);
    if (space == NULL)
    {
        ThrowMemError(__FILE__, __func__, __LINE__);
        exit(-1);
    }
    insertToTable(space, size);
    return space;
}

size_t getAllocations()
{
    return allocations;
}