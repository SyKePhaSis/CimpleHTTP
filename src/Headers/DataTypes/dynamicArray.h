#ifndef _DYN_ARRAY
#define _DYN_ARRAY

/* ARRAY */

enum ARRAY_TYPE
{
    CHAR_ARR,
    INT_ARR
};

typedef struct Array
{
    void **items;
    enum ARRAY_TYPE at;
    long size;
} Array;

Array *getArray(enum ARRAY_TYPE ar);
void addToArray(Array *arr, void *item);
void *getFromArray(Array *arr, long index);
void removeFromArray(Array *arr, long index);
long findInArray(Array *arr, void *item);
void freeArray(Array *arr);
void printArray(Array *arr);

/* ARRAY ITERATOR */

typedef struct Iterator
{
    Array *arr;
    long index;
} Iterator;

Iterator *createIterator(Array *arr);
int iteratorHasNext(Iterator *it); // Should not modify iterator
void *iteratorGetNext(Iterator *it);
void *iteratorRemove(Iterator *it);
void iteratorCleanup(Iterator *it);

#endif
