#ifndef _BUFFER
#define _BUGGER

#include <stdlib.h>

typedef struct Buffer
{
    void *buf;
    size_t size;
} Buffer;

Buffer *createBuffer();
void addToBuffer(Buffer *bufer, void *block, size_t size);
void *flushBuffer(Buffer *bufer);
char *flushBufferAsString(Buffer *bufer);
void freeBuffer(Buffer *buffer);

#endif