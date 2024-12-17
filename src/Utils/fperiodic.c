#include "DataTypes/types.h"
#include "DataTypes/dictionary.h"
#include "Utils/fperiodic.h"
#include "Utils/logger.h"
#include "Utils/memmory.h"
#include <stdarg.h>

/* HELPERS */

void cleanupClassifier(void *item, ITEM_TYPE it)
{
    switch (it)
    {
    case STR:
        deallocate(item);
        break;
    case INTEGER:
        deallocate(item);
        break;
    case DICT:
        cleanupDictionary(item);
        break;
    case ARRAY:
        freeArray(item);
        break;
    }
}

/* EXPORTED FUNCTIONS */

int null_validation(int n, ...)
{
    va_list vlist;
    int ret = 1;
    va_start(vlist, n);
    for (int i = 0; i < n; i++)
    {
        void *val = va_arg(vlist, void *);
        if (!val)
        {
            ret = 0;
            logWarning("[%s:%s] Validation item was NULL", __FILE__, __func__);
            break;
        }
    }
    va_end(vlist);
    return ret;
}

void pcleanup(int n, ...)
{
    va_list vlist;
    va_start(vlist, n);
    for (int i = 0; i < n; i++)
    {
        TypePair val = va_arg(vlist, TypePair);
        if (!val.item)
        {
            logWarning("[%s:%s] Item was NULL", __FILE__, __func__);
            continue;
        }
        cleanupClassifier(val.item, val.it);
    }
    va_end(vlist);
}