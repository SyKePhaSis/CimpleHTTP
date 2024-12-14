#include "Utils/memmory.h"
#include "Utils/logger.h"
#include "DataTypes/dictionary.h"
#include "DataTypes/dynamicArray.h"

#include <string.h>
#include <stdlib.h>

Dict *createDict()
{
    Dict *dict = allocate(sizeof(Dict));
    Array *keys = getArray(CHAR_ARR);
    Array *values = getArray(CHAR_ARR);
    dict->keys = keys;
    dict->values = values;
    dict->count = 0;
    return dict;
}

void addToDict(Dict *dict, char *key, void *value)
{
    if (key && value && dict)
    {
        // if (isKeyInDict(dict, key))
        // {
        //     logWarning("Key tried added is already in dictionary");
        //     // TODO: IMPLEMENT OVERWRITTING KEY-VALUE PAIR
        // }
        addToArray(dict->keys, key);
        addToArray(dict->values, value);
        dict->count++;
        logInfo("Added Value to Dictionary");
        return;
    }
    logError("Some Value Passed as Parameter is NULL");
}

void removeFromDict(Dict *dict, char *key)
{
    Iterator *it = createIterator(dict->keys);
    while (iteratorHasNext(it))
    {
        if (strcmp((char *)iteratorGetNext(it), key) == 0)
        {
            removeFromArray(dict->keys, it->index);
            removeFromArray(dict->values, it->index);
            dict->count--;
            break;
        }
    }
    iteratorCleanup(it);
}

void *getValueFromDict(Dict *dict, char *key)
{
    Iterator *it = createIterator(dict->keys);
    void *ret = NULL;
    while (iteratorHasNext(it))
    {
        if (strcmp((char *)iteratorGetNext(it), key) == 0)
        {
            ret = getFromArray(dict->values, it->index);
            break;
        }
    }
    iteratorCleanup(it);
    return ret;
}

size_t getDictKeys(Dict *dict, char **keyArray, size_t size)
{
    Iterator *it = createIterator(dict->keys);
    char *val;
    size_t win = 0;
    while (iteratorHasNext(it))
    {
        val = (char *)iteratorGetNext(it);
        if (val)
            // strcpy(keyArray[win++], val);
            keyArray[win++] = val;
    }
    iteratorCleanup(it);
    return win;
}

int isKeyInDict(Dict *dict, char *key)
{
    Iterator *it = createIterator(dict->keys);
    int ret = 0;
    while (iteratorHasNext(it))
    {
        if (strcmp((char *)iteratorGetNext(it), key) == 0)
        {
            ret = 1;
            break;
        }
    }
    iteratorCleanup(it);
    return ret;
}

void cleanupDictionary(Dict *dict)
{
    freeArray(dict->keys);
    freeArray(dict->values);
    deallocate(dict);
}

// Works only for string -> Migration needed for other types
// Rewrite by getting pairs rather than keys and then values
char *flushDictToJson(Dict *dict)
{
    size_t i = 0;
    char *keys[10];
    char *buf = allocate(1000); // -> WILL BE CHANGED FOR DYNAMIC BUFFER
    buf[0] = 0x00;
    strcat(buf, "{\n");

    size_t key_count = getDictKeys(dict, keys, 10);
    logInfo("Key-count: %llu", key_count);
    while (i < key_count)
    {
        strcat(buf, "\t\"");
        strcat(buf, keys[i]);
        strcat(buf, "\" : \"");
        strcat(buf, getValueFromDict(dict, keys[i]));
        strcat(buf, "\"\n");
        i++;
    }
    strcat(buf, "}");
    logInfo("Resjson: %s", buf);
    return buf;
}

void printDictionary(Dict *dict)
{
}