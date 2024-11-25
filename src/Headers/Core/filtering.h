#ifndef _FILTERING
#define _FILTERING

#include "Utils/dynamicArray.h"

Array *getWhitelist(void);
Array *getBlacklist(void);
void initializeWhitelist(void);
void initializeBlacklist(void);

#endif