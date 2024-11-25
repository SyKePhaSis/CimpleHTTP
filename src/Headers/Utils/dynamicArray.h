#ifndef _DYN_ARRAY
#define _DYN_ARRAY

#define findInArrray

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
void removeFromArray(Array *arr, long index);
long findInArray(Array *arr, void *item);
void freeArray(Array *arr);
void printArray(Array *arr);

#endif
