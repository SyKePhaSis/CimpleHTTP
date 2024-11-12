#include "Utils/errors.h"
#include "Utils/logger.h"

void ThrowMemError(char *file, const char *func, int line)
{
    logError("[%s:%s:%d] Memmory Error Occured", file, func, line);
}