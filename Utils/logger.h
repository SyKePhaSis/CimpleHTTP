#ifndef _LOGGER
#define _LOGGER

#include<stdarg.h>

void logInfo(char* line,...);
void logError(char* line,...);
void logWarning(char* line,...);
void logSuccess(char* line, ...);
void __log(char* msg, char* line, va_list vlist);

// COLOURS
void red();
void yellow();
void green();
void reset();

#endif