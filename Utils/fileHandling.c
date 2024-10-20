#include<stdio.h>
#include<sys/stat.h>
#include<string.h>
#include<stdlib.h>

#include"Utils/fileHandling.h"
#include"Utils/logger.h"

FileResp getFile(const char* path)
{
    FileResp res;
    char _buf[MAX_PATH_SIZE];
    strcpy(_buf, HTML_LOCATION);
    logInfo("%s", path);
    strcat(_buf, path);
    if(!fileExists(_buf)){
        logError("Couldn't find file in location: %s", _buf);
        res.found = 0;
        return res;
    }
    res.fp = fopen(_buf, F_READ);
    if(res.fp == NULL)
    {
        logError("Couldn't open file in location: %s", _buf);
        res.found = 0;
        return res;
    }
    res.len = getFileLen(res.fp);
    res.found = 1;
    res.data = (char*)malloc(sizeof(char)*(res.len - 1));
    fread(res.data, 1, res.len, res.fp);
    return res;
}

int fileExists(const char* path)
{
    struct stat _Stat;
    return (stat(path, &_Stat)) == 0;
}

long getFileLen(FILE* fp)
{
    long f_size;
    fseek(fp, 0,SEEK_END);
    f_size = ftell(fp);
    rewind(fp);
    return (unsigned long long) f_size;
}