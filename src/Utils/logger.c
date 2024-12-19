#include <stdio.h>
#include <stdarg.h>
#include "Utils/logger.h"

enum LOGGING_LEVEL loglev = Connection;

void logInfo(const char *line, ...)
{
    if (Info <= loglev)
    {
        const char *msg = "[INFO] ";
        va_list argp;
        va_start(argp, line);
        __log(msg, line, argp);
        va_end(argp);
    }
    return;
}

void logError(const char *line, ...)
{
    if (Error <= loglev)
    {
        const char *msg = "[ERROR] ";
        va_list argp;
        va_start(argp, line);
        red();
        __log(msg, line, argp);
        reset();
        va_end(argp);
    }
    return;
}

void logWarning(const char *line, ...)
{
    if (Warning <= loglev)
    {
        const char *msg = "[WARNING] ";
        va_list argp;
        va_start(argp, line);
        yellow();
        __log(msg, line, argp);
        reset();
        va_end(argp);
    }
    return;
}

void logSuccess(const char *line, ...)
{
    if (Success <= loglev)
    {
        const char *msg = "[SUCCESS] ";
        va_list argp;
        va_start(argp, line);
        green();
        __log(msg, line, argp);
        reset();
        va_end(argp);
    }
    return;
}

void logConnection(const char *line, ...)
{
    if (Connection <= loglev)
    {
        const char *msg = "[CONNECTION] ";
        va_list argp;
        va_start(argp, line);
        __log(msg, line, argp);
        va_end(argp);
    }
}

void __log(const char *msg, const char *line, va_list vlist)
{
    printf("%s", msg);
    vfprintf(stdout, line, vlist);
    printf("\n");
}

void setLogLevel(enum LOGGING_LEVEL level)
{
    reset();
    loglev = level;
}

enum LOGGING_LEVEL getLogLevel()
{
    return loglev;
}

// COLOUR FUNCTIONS

void red()
{
    printf("\033[0;31m");
    return;
}

void yellow()
{
    printf("\033[0;33m");
    return;
}

void green()
{
    printf("\033[0;32m");
    return;
}

void reset()
{
    printf("\033[37m");
    return;
}