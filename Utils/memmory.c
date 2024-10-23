#include "Utils/memmory.h"
#include "Utils/logger.h"
#include <stdlib.h>

size_t allocations = 0;

void *allocate(size_t size)
{
    void *space = malloc(size);
    if (space == NULL)
    {
        logError("Couldn't Allocate Space");
        exit(-1);
    }
    allocations++;
    return space;
}

void deallocate(void *block)
{
    if (block != NULL)
    {
        free(block);
        block = NULL;
        allocations--;
    }
    else
    {
        logError("Space was NULL, couldn't deallocated");
    }
}

void *reallocate(void *block, size_t size)
{
    void *space = realloc(block, size);
    if (space == NULL)
    {
        logError("Reallocate Allocate Space");
        exit(-1);
    }
    return space;
}

size_t getAllocations()
{
    return allocations;
}