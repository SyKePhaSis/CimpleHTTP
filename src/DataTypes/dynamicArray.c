#include "Utils/memmory.h"
#include "Utils/logger.h"
#include "DataTypes/dynamicArray.h"
#include "DataTypes/types.h"

#include <string.h>

/* HELPERS */

size_t getSizeOfObject(void *item, ARRAY_TYPE at)
{
    switch (at)
    {
    case CHAR_ARR:
        return strlen(item) + 1;

    case INT_ARR:
        return sizeof(int);

    case ITEM_TYPE_ARR:
        return sizeof(ITEM_TYPE);
    }
}

int cmpObjects(void *item1, void *item2, ARRAY_TYPE at)
{
    switch (at)
    {
    case CHAR_ARR:
        return strcmp((char *)item1, (char *)item2) == 0;
    case INT_ARR:
        return *(int *)item1 == *(int *)item2;
    case ITEM_TYPE_ARR:
        return *(ITEM_TYPE *)item1 == *(ITEM_TYPE *)item2;
    }
}

/*         */

Array *getArray(ARRAY_TYPE at)
{
    Array *arr = allocate(sizeof(Array));
    arr->at = at;
    arr->items = NULL;
    arr->size = 0;
    return arr;
}

void addToArray(Array *arr, void *item)
{
    for (long i = 0; i < arr->size; i++)
    {
        if (arr->items[i] == NULL)
        {
            logInfo("Found null item in array to replace");
            arr->items[i] = item;
            return;
        }
    }
    if (arr->items == NULL)
    {
        if (arr->size != 0)
        {
            logWarning("Dynamic Array was null with size greater than one");
            arr->size = 0;
        }
        arr->items = allocate(sizeof(item));
    }
    else
    {
        arr->items = reallocate(arr->items, sizeof(item) * (arr->size + 1));
    }
    arr->items[arr->size] = allocate(getSizeOfObject(item, arr->at));
    memcpy(arr->items[arr->size++], item, getSizeOfObject(item, arr->at));
    logInfo("Item Added To Dynamic Array");
}

void removeFromArray(Array *arr, long index)
{
    arr->items[index] = NULL;
}

long findInArray(Array *arr, void *item)
{
    for (long i = 0; i < arr->size; i++)
    {
        if (cmpObjects(item, arr->items[i], arr->at))
        {
            return i;
        }
    }
    return -1;
}

void freeArray(Array *arr)
{
    if (arr)
    {
        for (long i = 0; i < arr->size; i++)
        {
            if (arr->items[i] != NULL)
            {
                deallocate(arr->items[i]);
            }
        }
        deallocate(arr->items);
        deallocate(arr);
    }
}

void *getFromArray(Array *arr, long index)
{
    if (index < arr->size)
    {
        return arr->items[index];
    }
    logWarning("Tried to access element outside of bounds of Dynamic Array");
    return NULL;
}

/* ITERATOR FUNCTIONS */

Iterator *createIterator(Array *arr)
{
    if (arr)
    {
        Iterator *it = allocate(sizeof(Iterator));
        it->arr = arr;
        it->index = -1;
        return it;
    }
    logError("Parameter was null");
    return NULL;
}

int iteratorHasNext(Iterator *it)
{
    long n_index = it->index;
    void *val = NULL;
    while (++n_index < it->arr->size)
    {
        val = getFromArray(it->arr, n_index);
        if (val)
            return 1;
    }
    return 0;
}

void *iteratorGetNext(Iterator *it)
{
    if (iteratorHasNext(it))
    {
        long n_index = it->index + 1;
        void *val = NULL;
        while (n_index < it->arr->size)
        {
            val = getFromArray(it->arr, n_index);
            if (val)
            {
                it->index = n_index;
                return val;
            }
            n_index++;
        }
    }
    return NULL;
}

void *iteratorRemove(Iterator *it)
{
    removeFromArray(it->arr, it->index);
}

void iteratorCleanup(Iterator *it)
{
    deallocate(it);
}

/* HELPER FUNCTIONS */

void printArray(Array *arr)
{
    logInfo("Priting Item Array");
    logInfo("Array Size: %d", arr->size);
    for (long i = 0; i < arr->size; i++)
    {
        if (arr->items[i] != NULL)
        {
            if (arr->at == CHAR_ARR)
            {
                logInfo("Item: %s", (char *)arr->items[i]);
            }
            else if (arr->at == INT_ARR)
            {
                logInfo("Item: %d", *((int *)arr->items[i]));
            }
        }
    }
}