#include "Utils/httpCreator.h"
#include "Utils/logger.h"
#include "Utils/fileHandling.h"
#include "Utils/memmory.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>

httpResponse getHttpReq(void)
{
    httpResponse htr;

    htr.info.array = allocate(3 * sizeof(char *));
    htr.info.length = 0;
    htr.info.size = 0;

    htr.headers.array = NULL;
    htr.headers.length = 0;
    htr.headers.size = 0;

    htr.body = NULL;
    return htr;
}

void addBody(httpResponse *res, FileResp *fres)
{
    res->body = fres->data;
    addHeaderFormated(res, "Content-Length: %llu", strlen(fres->data) * sizeof(char));
    logInfo("Added Body");
}

void addMethod(httpResponse *res, const char *method)
{
    addInfo(res, method);
    logInfo("Added Method %s", method);
}

void addResCode(httpResponse *res, const char *code)
{
    addInfo(res, code);
    logInfo("Added Response Code: %s", code);
}

void addVersion(httpResponse *res, const char *version)
{
    addInfo(res, version);
    logInfo("Added Version: %s", version);
}

void addInfo(httpResponse *res, const char *line)
{
    res->info.array[res->info.length] = (char *)allocate(sizeof(char) * (strlen(line) + 1));
    if (res->info.array[res->info.length])
    {
        strcpy(res->info.array[res->info.length], line);
        res->info.length++;
        res->info.size += sizeof(char) * strlen(line);
        return;
    }
    logError("Couldn't allocate memmory to store info");
    return;
}

void addHeaderFormated(httpResponse *res, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char buff[_FORMATED_HEAD_BUF_SIZE];

    if (vsnprintf(buff, _FORMATED_HEAD_BUF_SIZE, fmt, args) < 0)
    {
        logError("Couldn't format Header for http Response");
    }
    va_end(args);
    logInfo("Added Formated Header");
    addHeader(res, buff);
}

void addHeader(httpResponse *res, const char *line)
{
    if (res->headers.array == NULL)
    {
        res->headers.array = allocate(sizeof(char *) * (res->headers.length + 1));
    }
    else
    {
        res->headers.array = reallocate(res->headers.array, sizeof(char *) * (res->headers.length + 1));
    }
    res->headers.array[res->headers.length] = allocate(sizeof(char) * (strlen(line) + 1));
    strcpy(res->headers.array[res->headers.length], line);
    res->headers.size += sizeof(char) * (strlen(line) + 1);
    res->headers.length += 1;
    logInfo("Added Header: %s", res->headers.array[res->headers.length - 1]);
    return;
}

char *flushHttpRes(httpResponse *res)
{
    if (proper(res))
    {
        char *buf = NULL;
        logInfo("Http Response is Proper");
        if (res->body != NULL)
        {
            logInfo("Alocating %lu bytes to memmory", strlen(res->body) * sizeof(char) + 2000);
            buf = allocate(strlen(res->body) * sizeof(char) + 500);
        }
        else
        {
            logError("Body was null");
        }
        buf[0] = 0x00;
        for (size_t i = 0; i < res->info.length; i++)
        {
            strcat(buf, res->info.array[i]);
            strcat(buf, " ");
        }
        addCRLF(buf);
        for (size_t j = 0; j < res->headers.length; j++)
        {
            strcat(buf, res->headers.array[j]);
            if (j != res->headers.length)
            {
                strcat(buf, "\n");
            }
        }
        addCRLF(buf);
        addCRLF(buf);
        logInfo("Adding Body");
        strcat(buf, res->body);
        dealloacteHttpObj(res);
        logInfo("Flushed Request");
        return buf;
    }
    dealloacteHttpObj(res);
    return NULL;
}

int proper(httpResponse *res)
{

    if (res->body == NULL)
    {
        logError("Http Request cannot be flushed, has no body!");
        return 0;
    }
    if (res->headers.array == NULL)
    {
        logError("Http Request cannot be flushed, has no headers!");
        return 0;
    }
    if (res->info.array == NULL)
    {
        logError("Http Request cannot be flushed, has no info!");
        return 0;
    }
    return 1;
}

void addCRLF(char *str)
{
    strcat(str, "\r\n");
    return;
}

void dealloacteHttpObj(httpResponse *res)
{
    logInfo("dealloacteing Headers Array");
    for (size_t i = 0; i < res->headers.length; i++)
    {
        deallocate(res->headers.array[i]);
        res->headers.array[i] = NULL;
    }
    deallocate(res->headers.array);
    res->headers.array = NULL;
    logInfo("dealloacteing Info Array");
    for (size_t i = 0; i < res->info.length; i++)
    {
        deallocate(res->info.array[i]);
        res->info.array[i] = NULL;
    }
    deallocate(res->info.array);
    res->info.array = NULL;
    logInfo("dealloacted Object");
}

void printHttpRes(httpResponse *res)
{
    if (proper(res))
    {
        logInfo("Printing Flushed Res: ");
        char *flushed = flushHttpRes(res);
        if (flushed != NULL)
        {
            printf("%s", flushed);
        }
        else
        {
            logWarning("Couldn't print flushed Http Response");
        }
    }
}
