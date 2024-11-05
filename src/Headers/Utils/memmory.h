#ifndef _MEMMORY
#define _MEMMORY

#include <stdlib.h>

void *allocate(size_t size);
void deallocate(void *block);
void *reallocate(void *block, size_t size);
size_t getAllocations(void);

#endif