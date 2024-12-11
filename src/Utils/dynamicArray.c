#include "Utils/dynamicArray.h"
#include "Utils/memmory.h"
#include "Utils/logger.h"

#include <string.h>

Array *getArray(enum ARRAY_TYPE at)
{
    Array *arr = allocate(sizeof(Array));
    arr->at = at;
    arr->items = NULL;
    arr->size = 0;
    return arr;
}

void addToArray(Array *arr, void *item)
{
    if (arr->at == CHAR_ARR)
    {
        for (long i = 0; i < arr->size; i++)
        {
            if ((char *)arr->items[i] == NULL)
            {
                logInfo("Found null item in array to replace");
                arr->items[i] = (char *)item;
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
            arr->items = allocate(sizeof((char *)item));
        }
        else
        {
            arr->items = reallocate(arr->items, sizeof((char *)item) * (arr->size + 1));
        }
        arr->items[arr->size] = allocate(strlen(item) + 1);
        strcpy(arr->items[arr->size++], item);
        logInfo("Item Added To Dynamic Array");
    }
    else if (arr->at == INT_ARR)
    {
        for (long i = 0; i < arr->size; i++)
        {
            if ((int *)arr->items[i] == NULL)
            {
                arr->items[i] = (int *)item;
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
            arr->items = allocate(sizeof((int *)item));
        }
        else
        {
            arr->items = reallocate(arr->items, sizeof((int *)item) * (arr->size + 1));
        }
        arr->items[arr->size] = allocate(sizeof((int *)item));
        arr->items[arr->size++] = item;
        logInfo("Item Added To Dynamic Array");
    }
}

void removeFromArray(Array *arr, long index)
{
    arr->items[index] = NULL;
}

long findInArray(Array *arr, void *item)
{
    if (arr->at == CHAR_ARR)
    {
        for (long i = 0; i < arr->size; i++)
        {
            if (strcmp((char *)arr->items[i], (char *)item) == 0)
            {
                return i;
            }
        }
    }
    else if (arr->at == INT_ARR)
    {
        for (long i = 0; i < arr->size; i++)
        {
            if (*((int *)arr->items[i]) == *((int *)item))
            {
                return i;
            }
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