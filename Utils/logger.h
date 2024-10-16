#pragma once
#ifndef _LOGGER
#define _LOGGER

#include<stdarg.h>

void logInfo(const char* line,...);
void logError(const char* line,...);
void logWarning(const char* line,...);
void logSuccess(const char* line, ...);
void __log(const char* msg, const char* line, va_list vlist);

// COLOURS
void red();
void yellow();
void green();
void reset();

#endif