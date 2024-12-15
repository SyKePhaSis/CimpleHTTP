#include <stdlib.h>
#include <string.h>
#include "DataTypes/buffer.h"
#include "Utils/memmory.h"
#include "Utils/logger.h"

Buffer *createBuffer()
{
    Buffer *buf = allocate(sizeof(Buffer));
    buf->size = 0;
    buf->buf = NULL;
    return buf;
}

void addToBuffer(Buffer *buffer, void *block, size_t size)
{
    if (buffer->buf == NULL)
    {
        if (buffer->size != 0)
        {
            logWarning("Buffer was NULL but size was non-zero");
        }
        buffer->buf = allocate(size);
        buffer->size = size;

        memcpy(buffer->buf, block, size);
    }
    else
    {
        buffer->buf = reallocate(buffer->buf, buffer->size + size);
        memcpy((char *)buffer->buf + buffer->size, block, size);
        buffer->size = buffer->size + size;
    }
}

void *flushBuffer(Buffer *buffer)
{
    void *ret_buf = allocate(buffer->size);
    memcpy(ret_buf, buffer->buf, buffer->size);
    freeBuffer(buffer);
}

char *flushBufferAsString(Buffer *buffer)
{
    char *ret_buf = allocate(buffer->size + 1);
    memcpy(ret_buf, buffer->buf, buffer->size);
    memcpy(ret_buf + buffer->size, "\000", 1);
    freeBuffer(buffer);
    return ret_buf;
}

void freeBuffer(Buffer *buffer)
{
    mass_deallocation(2, buffer, buffer->buf);
}