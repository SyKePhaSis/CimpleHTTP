#ifndef _FILE_MANAGER
#define _FILE_MANAGER

#include <stdio.h>

#define MAX_PATH_SIZE 512
#define HTML_LOCATION "./Webpage/html/"
#define CSS_LOCATION "./Webpage/assets/css/"
#define JS_LOCATION "./Webpage/assets/js/"
#define TTF_LOCATION "./Webpage/assets/ttf/"
#define IMAGE_LOCATION "./Webpage/assets/images/"
#define F_READ "rb"

typedef struct FileResp
{
    FILE *fp;
    char *path;
    void *data;
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