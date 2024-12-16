#ifndef _BUFFER
#define _BUGGER

#include <stdlib.h>
#include "DataTypes/types.h"

Buffer *createBuffer();
void addToBuffer(Buffer *bufer, void *block, size_t size);
void *flushBuffer(Buffer *bufer);
char *flushBufferAsString(Buffer *bufer);
void freeBuffer(Buffer *buffer);

#endif