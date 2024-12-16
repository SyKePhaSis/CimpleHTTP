#ifndef _DYN_ARRAY
#define _DYN_ARRAY

#include "DataTypes/types.h"

/* ARRAY */

Array *getArray(ARRAY_TYPE ar);
void addToArray(Array *arr, void *item);
void *getFromArray(Array *arr, long index);
void removeFromArray(Array *arr, long index);
long findInArray(Array *arr, void *item);
void freeArray(Array *arr);
void printArray(Array *arr);

/* ARRAY ITERATOR */

Iterator *createIterator(Array *arr);
int iteratorHasNext(Iterator *it); // Should not modify iterator
void *iteratorGetNext(Iterator *it);
void *iteratorRemove(Iterator *it);
void iteratorCleanup(Iterator *it);

#endif
