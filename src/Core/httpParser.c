#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "Core/httpParser.h"
#include "Utils/memmory.h"
#include "Utils/logger.h"

sreq splitReq(char *req)
{
    logInfo("Splitting the Request");
    sreq resp;
    resp.finished = 0;
    char **out = allocate(sizeof(char *));

    char *line;
    int c = 0;
    line = strtok(req, "\r\n");
    logInfo("Check");
    while (line != NULL)
    {
        c++;
        out = reallocate(out, c * sizeof(char *));
        out[c - 1] = allocate(sizeof(char) * (strlen(line) + 1));
        strcpy(out[c - 1], line);
        line = strtok(NULL, "\r\n");
    }
    resp.finished = 1;
    resp.s_arr = out;
    resp.len = c;
    return resp;
}

void getInfo(request *req, char *line)
{
    char **fs = allocate(sizeof(char *) * 3);
    char *tok;
    int c = 0;
    tok = strtok(line, " ");
    while (tok != NULL)
    {
        if (c < 3)
        {
            fs[c] = allocate((strlen(tok) + 1) * sizeof(char));
            strcpy(fs[c], tok);
        }
        else
        {
            logError("Needed more space to store Method");
        }
        c++;
        tok = strtok(NULL, " ");
    }
    req->method = allocate(sizeof(char) * strlen(fs[0]) + 1);
    req->path = allocate(sizeof(char) * strlen(fs[1]) + 1);
    req->version = allocate(sizeof(char) * strlen(fs[2]) + 1);
    strcpy(req->method, fs[0]);
    strcpy(req->path, fs[1]);
    strcpy(req->version, fs[2]);
    for (int i = 0; i < 3; i++)
    {
        deallocate(fs[i]);
    }
    deallocate(fs);
}

// char* getPath(char* req){return NULL;}
// char* getQueryParams(char* req){return NULL;}
// int verifyRequest(char* req){return 0;}

request extractRequestInfo(char *req)
{
    request ret;
    ret.failed = 0;
    sreq split_req = splitReq(req);
    if (!split_req.finished)
    {
        return ret;
    }
    getInfo(&ret, split_req.s_arr[0]);
    dealloacteSREQ(split_req);
    return ret;
}

void dealloacteSREQ(sreq split_req)
{
    for (int i = 0; i < split_req.len; i++)
    {
        deallocate(split_req.s_arr[i]);
        split_req.s_arr[i] = NULL;
    }
    deallocate(split_req.s_arr);
    logInfo("dealloacted SREQ Object");
}

void deallocateReq(request *req)
{
    deallocate(req->version);
    deallocate(req->method);
    deallocate(req->path);
}

// Helpers
void printSreq(sreq req)
{
    for (int i = 0; i < req.len; i++)
    {
        printf("%d: %s \n", i, *(req.s_arr + i));
    }
    return;
}
