#pragma once
#ifndef _FILE_MANAGER
#define _FILE_MANAGER

#include <stdio.h>

#define MAX_PATH_SIZE 512
#define HTML_LOCATION "./Webpage/html/"
#define CSS_LOCATION "./Webpage/assets/css/"
#define JS_LOCATION "./Webpage/assets/js/"
#define TTF_LOCATION "./Webpage/assets/ttf/"
#define F_READ "r"

typedef struct FileResp
{
    FILE *fp;
    char *path;
    char *data;
    int found;
    int len;
} FileResp;

FileResp getFile(const char *path);
int fileExists(const char *path);
long getFileLen(FILE *fp);
char *getFileName(char *path);
char *getFileExt(char *name);
void dealloacteFileResp(FileResp *fileResp);
char *getFilePath(char *ext);

#endif