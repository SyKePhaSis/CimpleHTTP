#include "Utils/memmory.h"
#include "Utils/logger.h"
#include "DataTypes/types.h"
#include "DataTypes/dynamicArray.h"

#include <string.h>

/* HELPER */

size_t getSizeOfItemObject(void *item, ITEM_TYPE it)
{
    switch (it)
    {
    case STR:
        return strlen(item) + 1;
    case INTEGER:
        return sizeof(int);
    case DICT:
        return sizeof(Dict);
    case ARRAY:
        return sizeof(Array);
    }
}

void deallocateObject(void *item, ITEM_TYPE it)
{
}

/*       */

GenericArray *getGenericArray()
{
    GenericArray *ga = allocate(sizeof(GenericArray));
    ga->items = NULL;
    ga->at = getArray(ITEM_TYPE_ARR);
    ga->size = 0;
}

void addToGenericArray(GenericArray *ga, void *item, ITEM_TYPE it)
{
    for (long i = 0; i < ga->size; i++)
    {
        if (ga->items[i] == NULL)
        {
            logInfo("Found null item in array to replace");
            ga->items[i] = item;
            addToArray(ga->at, &it);
            return;
        }
    }
    if (ga->items == NULL)
    {
        if (ga->size != 0)
        {
            logWarning("Dynamic Array was null with size greater than one");
            ga->size = 0;
        }
        ga->items = allocate(sizeof(item));
    }
    else
    {
        ga->items = reallocate(ga->items, sizeof(item) * (ga->size + 1));
    }
    ga->items[ga->size] = allocate(getSizeOfItemObject(item, it));
    addToArray(ga->at, &it);
    memcpy(ga->items[ga->size++], item, getSizeOfItemObject(item, it));
    logInfo("Item Added To Dynamic Array");
}

void removeFromGenericArray(GenericArray *ga, long index)
{
    ga->items[index] = NULL;
    removeFromArray(ga->at, index);
}

void *getFromGenericArray(GenericArray *ga, long index)
{
    if (index < ga->size)
    {
        return ga->items[index];
    }
    logWarning("Tried to access element outside of bounds of Dynamic Array");
    return NULL;
}

void *getTypeFromGenericArray(GenericArray *ga, long index)
{
    return getFromArray(ga->at, index);
}

void freeGenericArray(GenericArray *ga)
{
    if (ga)
    {
        freeArray(ga->at);
        for (long i = 0; i < ga->size; i++)
        {
            if (ga->items[i] != NULL)
            {
                deallocate(ga->items[i]);
            }
        }
        deallocate(ga->items);
        deallocate(ga);
    }
}