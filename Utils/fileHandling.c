#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>

#include "Utils/fileHandling.h"
#include "Utils/logger.h"
#include "Utils/memmory.h"

FileResp getFile(const char *path)
{
    FileResp res;
    char _buf[MAX_PATH_SIZE];
    char *ext = getFileExt(path);
    if (strcmp(ext, "html") == 0)
    {
        strcpy(_buf, HTML_LOCATION);
    }
    else if (strcmp(ext, "css") == 0)
    {
        strcpy(_buf, CSS_LOCATION);
    }
    else
    {
        strcpy(_buf, HTML_LOCATION);
    }
    logInfo("%s", path);
    strcat(_buf, path);
    if (!fileExists(_buf))
    {
        logError("Couldn't find file in location: %s", _buf);
        res.found = 0;
        return res;
    }
    res.fp = fopen(_buf, F_READ);
    if (res.fp == NULL)
    {
        logError("Couldn't open file in location: %s", _buf);
        res.found = 0;
        return res;
    }
    res.len = getFileLen(res.fp);
    res.found = 1;
    res.data = allocate(sizeof(char) * (res.len - 1));
    logInfo("Filesize: %lu", res.len);
    fread(res.data, 1, res.len, res.fp);
    return res;
}

int fileExists(const char *path)
{
    struct stat _Stat;
    return (stat(path, &_Stat)) == 0;
}

long getFileLen(FILE *fp)
{
    long f_size;
    fseek(fp, 0, SEEK_END);
    f_size = ftell(fp);
    rewind(fp);
    return (unsigned long long)f_size;
}

char *getFileName(char *path)
{
    char *filename = strrchr(path, '/');
    if (filename == NULL)
        filename = path;
    else
        filename++;
    return filename;
}

char *getFileExt(char *name)
{
    char *ext = strrchr(name, '.');
    if (ext == NULL)
        ext = name;
    else
        ext++;
    return ext;
}

void dealloacteFileResp(FileResp *fileResp)
{
    if (fileResp->data != NULL)
    {
        logInfo("dealloacteing fileResp data");
        deallocate(fileResp->data);
        fileResp->data = NULL;
        fclose(fileResp->fp);
        fileResp->fp = NULL;
    }
}