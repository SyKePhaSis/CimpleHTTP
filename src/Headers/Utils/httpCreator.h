#ifndef _HTTP_CREATOR
#define _HTTP_CREATOR

#include "fileHandling.h"
#include <stdarg.h>

#define _FORMATED_HEAD_BUF_SIZE 512
#define _RES_SIZE 100000

typedef struct dyn_arr
{
    char **array;
    long long unsigned int length;
    long long unsigned int size;
} dyn_arr;

typedef struct httpResponse
{
    dyn_arr info;
    dyn_arr headers;
    void *body;
    size_t body_size;
} httpResponse;

typedef struct resBuffer
{
    void *buffer;
    size_t size;
} resBuffer;

httpResponse
getHttpReq(void);
void addBody(httpResponse *res, FileResp *fres);
void addBodyEmpty(httpResponse *res);
void addVersion(httpResponse *res, const char *version);
void addMethod(httpResponse *res, const char *method);
void addResCode(httpResponse *res, const char *code);
void addHeader(httpResponse *res, const char *line);
void addHeaderFormated(httpResponse *res, const char *fmt, ...);
void addInfo(httpResponse *res, const char *line);
resBuffer flushHttpRes(httpResponse *res);
int proper(httpResponse *res);
void addCRLF(char *str);
void dealloacteHttpObj(httpResponse *res);

#endif