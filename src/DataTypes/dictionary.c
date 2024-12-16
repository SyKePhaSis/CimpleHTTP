#include "Utils/memmory.h"
#include "Utils/logger.h"
#include "Utils/fvalidation.h"
#include "DataTypes/buffer.h"
#include "DataTypes/dictionary.h"
#include "DataTypes/dynamicArray.h"
#include "DataTypes/genericArray.h"

#include <string.h>
#include <stdlib.h>

Dict *createDict()
{
    Dict *dict = allocate(sizeof(Dict));
    Array *keys = getArray(CHAR_ARR);
    GenericArray *values = getGenericArray();
    dict->keys = keys;
    dict->values = values;
    dict->count = 0;
    return dict;
}

void addToDict(Dict *dict, char *key, void *value, ITEM_TYPE it)
{
    if (null_validation(3, dict, key, value))
    {
        // if (isKeyInDict(dict, key))
        // {
        //     logWarning("Key tried added is already in dictionary");
        //     // TODO: IMPLEMENT OVERWRITTING KEY-VALUE PAIR
        // }
        addToArray(dict->keys, key);
        addToGenericArray(dict->values, value, it);
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
            removeFromGenericArray(dict->values, it->index);
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
            ret = getFromGenericArray(dict->values, it->index);
            break;
        }
    }
    iteratorCleanup(it);
    return ret;
}

Pair getPair(Dict *dict, char *key)
{
    Pair p;
    p.key = key;
    Iterator *it = createIterator(dict->keys);
    while (iteratorHasNext(it))
    {
        if (strcmp((char *)iteratorGetNext(it), key) == 0)
        {
            p.val = getFromGenericArray(dict->values, it->index);
            p.type = *(ITEM_TYPE *)getTypeFromGenericArray(dict->values, it->index);
            break;
        }
    }
    iteratorCleanup(it);
    return p;
}

void getDictKeys(Dict *dict, Array *keyArray)
{
    Iterator *it = createIterator(dict->keys);
    char *val = NULL;
    while (iteratorHasNext(it))
    {
        val = (char *)iteratorGetNext(it);
        if (val)
            addToArray(keyArray, val);
    }
    iteratorCleanup(it);
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
    freeGenericArray(dict->values);
    deallocate(dict);
}

// @TODO
// Works only for string -> Migration needed for other types
// Rewrite by getting pairs rather than keys and then values
char *flushDictToJson(Dict *dict)
{
    Array *keys = getArray(CHAR_ARR);
    getDictKeys(dict, keys);

    Buffer *buf = createBuffer();
    addToBuffer(buf, "{\n", 2);

    logInfo("Key-count: %llu", keys->size);
    Iterator *it = createIterator(keys);
    while (iteratorHasNext(it))
    {
        char *key = iteratorGetNext(it);
        Pair p = getPair(dict, key);
        if (p.val != NULL)
        {
            addToBuffer(buf, "\t\"", 2);
            addToBuffer(buf, key, strlen(key));
            addToBuffer(buf, "\" : ", 4);
            logInfo("TYPE: %d", p.type);
            switch (p.type)
            {
            case STR:
                addToBuffer(buf, "\"", 1);
                addToBuffer(buf, p.val, strlen(p.val));
                logInfo("TEST: %s", p.val);
                addToBuffer(buf, "\"", 1);
                if (iteratorHasNext(it))
                    addToBuffer(buf, ",", 1);
                addToBuffer(buf, "\n", 1);
                break;
            case DICT:
                char *obj = flushDictToJson((Dict *)(p.val));
                addToBuffer(buf, obj, strlen(obj));
                free(obj);
                break;
            }
        }
    }
    addToBuffer(buf, "}", 1);
    freeArray(keys);
    iteratorCleanup(it);
    return flushBufferAsString(buf);
}
