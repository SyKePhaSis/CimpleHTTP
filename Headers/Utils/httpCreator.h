#pragma once
#ifndef _HTTP_CREATOR
#define _HTTP_CREATOR

#include "fileHandling.h"
#include <stdarg.h>

#define _FORMATED_HEAD_BUF_SIZE 512
#define _RES_SIZE 10000

typedef struct dyn_arr {
    char** array;
    long long unsigned int length;
    long long unsigned int size;
} dyn_arr;

typedef struct httpResponse {
    dyn_arr info;
    dyn_arr headers;
    char* body;
} httpResponse;

httpResponse getHttpReq(void);
void addBody(httpResponse* res, FileResp* fres);
void addVersion(httpResponse* res, const char* version);
void addMethod(httpResponse* res, const char* method);
void addResCode(httpResponse* res, const char* code);
void addHeader(httpResponse* res, const char* line);
void addHeaderFormated(httpResponse* res, const char* fmt, ...);
void addInfo(httpResponse* res, const char* line);
char* flushHttpRes(httpResponse* res);
int proper(httpResponse* res);
void addCRLF(char* str);
void freeHttpObj(httpResponse* res);
void printHttpRes(httpResponse* res);

#endif