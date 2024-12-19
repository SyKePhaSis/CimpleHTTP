#ifndef DICTIONARY
#define _DICTIONARY

#include <stdlib.h>
#include "DataTypes/dynamicArray.h"
#include "DataTypes/types.h"

Dict *createDict();
void addToDict(Dict *dict, char *key, void *value, ITEM_TYPE it);
void addUnknownToDict(Dict *dict, char *key, void *value, size_t size);
void removeFromDict(Dict *dict, char *key);
void *getValueFromDict(Dict *dict, char *key);
Pair getPair(Dict *dict, char *key);
void getDictKeys(Dict *dict, Array *keyArray);
int isKeyInDict(Dict *dict, char *key);
char *flushDictToJson(Dict *dict);
void cleanupDictionary(Dict *dict);
#endif