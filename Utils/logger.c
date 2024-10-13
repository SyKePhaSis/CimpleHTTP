#include<stdio.h>
#include <stdarg.h>
#include"logger.h"


void logInfo(char* line, ...)
{
    char* msg = "[INFO] ";
    va_list argp;
    va_start(argp,line);
    __log(msg, line, argp);
    va_end(argp);
    return;
}

void logError(char* line, ...)
{
    char* msg = "[ERROR] ";
    va_list argp;
    va_start(argp,line);
    red();
    __log(msg, line, argp);
    reset();
    va_end(argp);
    return;
}

void logWarning(char* line, ...)
{
    char* msg = "[WARNING] ";
    va_list argp;
    va_start(argp,line);
    yellow();
    __log(msg, line, argp);
    reset();
    va_end(argp);
    return;
}

void logSuccess(char* line, ...)
{
    char* msg = "[SUCCESS] ";
    va_list argp;
    va_start(argp,line);
    green();
    __log(msg, line, argp);
    reset();
    va_end(argp);
    return;
}

void __log(char* msg, char* line, va_list vlist)
{
    printf(msg);
    vfprintf(stderr, line, vlist);
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