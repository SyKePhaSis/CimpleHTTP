#include "Utils/memmoryTable.h"
#include "Utils/logger.h"
#include "Utils/errors.h"

#include <stdlib.h>

MemT memTable = {.addresses = NULL, .sizes = NULL, .count = 0};

int findSpot(size_t *spot)
{
    for (size_t c = 0; c < memTable.count; c++)
    {
        if (memTable.addresses[c] == NULL)
        {
            *spot = c;
            return 1;
        }
    }
    return 0;
}

void insertToTable(void *address, size_t size)
{
    size_t spot;
    if (memTable.count == 0)
    {
        memTable.addresses = malloc(sizeof(void *));
        memTable.sizes = malloc(sizeof(size_t));
        memTable.addresses[memTable.count] = NULL;
        memTable.count++;
    }
    else if (!findSpot(&spot))
    {
        memTable.addresses = realloc(memTable.addresses, sizeof(void *) * (memTable.count + 1));
        memTable.addresses[memTable.count] = NULL;
        memTable.sizes = realloc(memTable.sizes, sizeof(void *) * (memTable.count + 1));
        memTable.count++;
    }
    if (memTable.addresses == NULL || memTable.sizes == NULL)
    {
        ThrowMemError(__FILE__, __func__, __LINE__);
        memTable.count--;
        exit(-1);
    }
    findSpot(&spot);
    memTable.addresses[spot] = address;
    memTable.sizes[spot] = size;
    return;
}

void removeFromTable(void *address)
{
    for (size_t c = 0; c < memTable.count; c++)
    {
        if (memTable.addresses[c] == address)
        {
            memTable.addresses[c] = NULL;
            memTable.sizes[c] = 0;
            return;
        }
    }
    logWarning("Couldn't find Address In Memmory Table");
}

void printMemTable()
{
    logInfo(" -------------------------------");
    logInfo("|         Memmory Table         |");
    logInfo(" -------------------------------");
    logInfo("|       Address      |   Size   |");
    logInfo(" -------------------------------");
    for (size_t c = 0; c < memTable.count; c++)
    {
        logInfo("| 0x%16p | %8llu |", memTable.addresses[c], memTable.sizes[c]);
        logInfo("-------------------------------");
    }
}