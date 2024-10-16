#include<stdio.h>
#include <stdarg.h>
#include"logger.h"


void logInfo(const char* line, ...)
{
    const char* msg = "[INFO] ";
    va_list argp;
    va_start(argp,line);
    __log(msg, line, argp);
    va_end(argp);
    return;
}

void logError(const char* line, ...)
{
    const char* msg = "[ERROR] ";
    va_list argp;
    va_start(argp,line);
    red();
    __log(msg, line, argp);
    reset();
    va_end(argp);
    return;
}

void logWarning(const char* line, ...)
{
    const char* msg = "[WARNING] ";
    va_list argp;
    va_start(argp,line);
    yellow();
    __log(msg, line, argp);
    reset();
    va_end(argp);
    return;
}

void logSuccess(const char* line, ...)
{
    const char* msg = "[SUCCESS] ";
    va_list argp;
    va_start(argp,line);
    green();
    __log(msg, line, argp);
    reset();
    va_end(argp);
    return;
}

void __log(const char* msg, const char* line, va_list vlist)
{
    printf("%s",msg);
    vfprintf(stdout, line, vlist);
    printf("\n");
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
    printf("\033[0m");
    return;
}