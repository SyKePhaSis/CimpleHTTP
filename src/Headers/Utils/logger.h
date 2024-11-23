#ifndef _LOGGER
#define _LOGGER

#include <stdarg.h>

enum LOGGING_LEVEL
{
    No = 0,
    Error = 1,
    Success = 2,
    Connection = 3,
    Warning = 4,
    Info = 5,
    All = 6
};

void logInfo(const char *line, ...);
void logError(const char *line, ...);
void logWarning(const char *line, ...);
void logSuccess(const char *line, ...);
void logConnection(const char *line, ...);
void __log(const char *msg, const char *line, va_list vlist);
void setLogLevel(enum LOGGING_LEVEL level);

// COLOURS
void red();
void yellow();
void green();
void reset();

#endif