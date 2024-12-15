#include "Utils/fvalidation.h"
#include "Utils/logger.h"
#include <stdarg.h>

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