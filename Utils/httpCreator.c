#include "httpCreator.h"
#include "logger.h"
#include "fileHandling.h"
#include<string.h>
#include<stdlib.h>
#include<stdarg.h>

httpResponse getHttpReq(void)
{
    httpResponse htr;

    htr.info.array = (char**)malloc(3*sizeof(char*));
    htr.info.length = 0;
    htr.info.size = 0;

    htr.headers.array = NULL;
    htr.headers.length = 0;
    htr.headers.size = 0;
    
    htr.body = NULL;
    return htr;
}

void addBody(httpResponse* res, FileResp* fres){
    res->body = fres->data;
    addHeaderFormated(res, "Content-Length: %llu", strlen(fres->data)*sizeof(char));
    logInfo("Added Body");
}

void addMethod(httpResponse* res, const char* method)
{
    addInfo(res, method);
    logInfo("Added Method %s", method);
}

void addResCode(httpResponse* res, const char* code)
{
    addInfo(res, code);
    logInfo("Added Response Code: %s", code);
}

void addVersion(httpResponse* res, const char* version)
{
    addInfo(res, version);
    logInfo("Added Version: %s", version);
}

void addInfo(httpResponse* res, const char* line)
{
    res->info.array[res->info.length] = (char*)malloc(sizeof(char)*strlen(line));
    strcpy(res->info.array[res->info.length], line);
    res->info.length++;
    res->info.size += sizeof(char)*strlen(line);
    return;
}

void addHeaderFormated(httpResponse* res, const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    char buff[_FORMATED_HEAD_BUF_SIZE];
    
    if(vsnprintf(buff, _FORMATED_HEAD_BUF_SIZE, fmt, args) < 0)
    {
        logError("Couldn't format Header for http Response");
    }
    va_end (args);
    addHeader(res,buff);
    logInfo("Added Formated Header");
}

void addHeader(httpResponse* res, const char* line)
{   

    if(res->headers.array == NULL)
    {
        if(res->headers.length > 0)
        {
            logInfo("it's null idk why...");
        } else {
            res->headers.array = (char**)malloc(sizeof(char*)*(res->headers.length + 1));
        }
    } else {
        res->headers.array = (char**)realloc(res->headers.array, sizeof(char*)*(res->headers.length + 1));
    }
    if(res->headers.array == NULL)
    {
        logError("Couldn't allocate memmory to headers array");
        return;
    }
    res->headers.array[res->headers.length] = (char*)malloc(sizeof(char)*strlen(line));
    strcpy(res->headers.array[res->headers.length], line);
    res->headers.size += sizeof(line);
    res->headers.length += 1;
    logInfo("Added Header: %s", res->headers.array[res->headers.length - 1]);
    return;
}

char* flushHttpRes(httpResponse* res)
{
    if(proper(res))
    {
        
        // char* buf = (char*)malloc(strlen(res->body)*sizeof(char) + res->info.size + res->headers.size + 6 + res->headers.length + 800);
        char* buf = malloc(_RES_SIZE*sizeof(char));
        if(buf == NULL)
        {
            logError("Couldn't allocate memmory to create response buffer");
            return NULL;
        }
        buf[0] = 0x00;
        for(size_t i = 0; i < res->info.length; i++)
        {
            strcat(buf, res->info.array[i]);
            strcat(buf, " ");
        }
        addCRLF(buf);
        for(size_t j = 0; j < res->headers.length; j++)
        {
            strcat(buf,res->headers.array[j]);
            if(j != res->headers.length)
            {
                strcat(buf, "\n");
            }
        }
        addCRLF(buf);
        addCRLF(buf);
        strcat(buf, res->body);
        freeHttpObj(res);
        logInfo("Flushed Request");
        return buf;
    }
    freeHttpObj(res);
    return NULL;
}  

int proper(httpResponse* res)
{
    
    if(res->body == NULL) 
    {
        logError("Http Request cannot be flushed, has no body!");
        return 0;
    }
    if(res->headers.array == NULL) 
    {
        logError("Http Request cannot be flushed, has no headers!");
        return 0;
    }
    if(res->info.array == NULL) 
    {
        logError("Http Request cannot be flushed, has no info!");
        return 0;
    }
    return 1;
}

void addCRLF(char* str)
{
    strcat(str, "\r\n");
    return;
}

void freeHttpObj(httpResponse* res)
{
    free(res->headers.array);
    free(res->info.array);
}

void printHttpRes(httpResponse* res)
{
    if(proper(res))
    {
        logInfo("Printing Flushed Res: ");
        char* flushed = flushHttpRes(res);
        if(flushed != NULL)
        {
            printf("%s", flushed);
        } else {
            logWarning("Couldn't print flushed Http Response");
        }
    } 
}
