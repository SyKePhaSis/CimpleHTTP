#ifndef DICTIONARY
#define _DICTIONARY

#include <stdlib.h>
#include "DataTypes/dynamicArray.h"

typedef struct Dict
{
    Array *keys;
    Array *values;
    // enum INPUT_TYPES **it; To identify
    long count;
} Dict;

Dict *createDict();
void addToDict(Dict *dict, char *key, void *value);
void removeFromDict(Dict *dict, char *key);
void *getValueFromDict(Dict *dict, char *key);
void getDictKeys(Dict *dict, Array *keyArray);
int isKeyInDict(Dict *dict, char *key);
char *flushDictToJson(Dict *dict);
void cleanupDictionary(Dict *dict);
#endif