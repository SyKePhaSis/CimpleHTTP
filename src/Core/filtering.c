#include "Core/filtering.h"
#include "Utils/memmory.h"
#include "Utils/dotenv.h"
#include "Utils/fileHandling.h"
#include "Utils/logger.h"
#include "DataTypes/dynamicArray.h"

#include <stdlib.h>
#include <string.h>

Array *blacklist = NULL;
Array *whitelist = NULL;

Array *getWhitelist(void)
{
    return whitelist;
}

Array *getBlacklist(void)
{
    return blacklist;
}

int inWhitelist(char *ip)
{
    if (findInArray(whitelist, ip) == -1)
        return 0;
    return 1;
}

int inBlacklist(char *ip)
{
    if (findInArray(blacklist, ip) == -1)
        return 0;
    return 1;
}

void processList(Array *arr, char *_buf)
{
    char *tok = strtok(_buf, "\r\n");
    while (tok != NULL)
    {
        if (tok[0] != 0x00 && getFirstChar(tok) != '#')
        {
            logInfo("tok: %s", tok);
            addToArray(arr, tok);
        }
        tok = strtok(NULL, "\r\n");
    }
    printArray(arr);
}

void getList(Array *arr, char *path)
{
    FILE *fp = fopen(path, "rb");
    char *_buf;
    long len;
    if (fp)
    {
        len = getFileLen(fp);
        _buf = allocate(sizeof(char) * len);
        fread(_buf, 1, len, fp);
        _buf[len] = 0x00;
        fclose(fp);
        processList(arr, _buf);
        return;
    }
    logError("Couldn't open list in path: %s", path);
}

void initializeWhitelist(void)
{
    char *defpath;
    whitelist = getArray(CHAR_ARR);
    if ((defpath = getEnvValue("WHITELIST_FILE")) != NULL)
    {
        getList(whitelist, defpath);
        return;
    }
    getList(whitelist, "configs/whitelist.txt");
}

void initializeBlacklist(void)
{
    char *defpath;
    blacklist = getArray(CHAR_ARR);
    if ((defpath = getEnvValue("BLACKLIST_FILE")) != NULL)
    {
        getList(blacklist, defpath);
        return;
    }
    getList(blacklist, "configs/blacklist.txt");
}
