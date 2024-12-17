#ifndef _DATA_TYPES
#define _DATA_TYPES

#include <stdlib.h>

/* BUFFER */
typedef struct Buffer
{
    void *buf;
    size_t size;
} Buffer;

/* ARRAY */
typedef enum
{
    CHAR_ARR,
    INT_ARR,
    ITEM_TYPE_ARR
} ARRAY_TYPE;

typedef struct Array
{
    void **items;
    ARRAY_TYPE at;
    long size;
} Array;

typedef struct Iterator
{
    Array *arr;
    long index;
} Iterator;

/* GENERIC ARRAY */

typedef struct GenericArray
{
    void **items;
    Array *at;
    long size;
} GenericArray;

/* DICTIONARY */

typedef enum ITEM_TYPE
{
    STR,
    INTEGER,
    DICT,
    ARRAY
} ITEM_TYPE;

typedef struct Dict
{
    Array *keys;
    GenericArray *values;
    long count;
} Dict;

typedef struct DictionaryIterator
{
    Dict *dict;
    long index;
} DictionaryIterator;

/* PAIRS */

typedef struct Pair
{
    char *key;
    void *val;
    ITEM_TYPE type;
} Pair;

typedef struct TypePair
{
    void *item;
    ITEM_TYPE it;
} TypePair;

/* FUNCTIONS */

TypePair getTypePair(void *item, ITEM_TYPE it);

#endif