#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include"httpParser.h"
#include"logger.h"

sreq splitReq(char* request)
{
    char **out = (char **)malloc(sizeof(char*)*_ARRAY_START_SIZE);
    if(out == NULL)
    {
        logError("Couldn't allocate memory to save request");
    }
    char *line;
    int c = 0;
    line = strtok(request, "\r\n");
    while(line != NULL)
    {

        if(c-1 >= _ARRAY_START_SIZE)
        {
            out = realloc(out, (c+1)*sizeof(char*));
            if(out == NULL)
            {
                logError("Couldn't append allocated memory to save request.");
            }
        }
        out[c] = (char*)malloc(sizeof(char) * (strlen(line) + 1));
        strcpy(out[c], line);
        if(out[c] == NULL)
        {
            logError("Couldn't Allocate Memmory to save Line");
        }
        line = strtok(NULL, " ");
        c++;
    }
    sreq resp; 
    resp.s_arr = out;
    resp.len = c;
    return resp;
}

request getMethod(char* line)
{
    char* fs[3] = {0};
    char* tok;
    int c = 0;
    tok = strtok(line, " ");
    while(tok != NULL)
    {
        if(c < 3) {
            fs[c] = (char*)malloc(strlen(tok)*sizeof(char));
            strcpy(fs[c], tok);
        } else {
            logError("Needed more space to store Method");
        }
        c++;
        tok = strtok(NULL, " ");
    }
    request req;
    req.code = fs[1];
    req.version = fs[0];
    return req;
}

char* getPath(char* request){};
char* getQueryParams(char* request){};
int verifyRequest(char* request){};

request extractRequestInfo(char* req)
{
    sreq sreq = splitReq(req);
    request ret = getMethod(sreq.s_arr[0]);
};

// Helpers

void printSreq(sreq req)
{
    for(size_t i = 0; i < req.len; i++)
    {
        printf("%d: %s \n", i,*(req.s_arr + i));
    }
    return;
}

void printReq(request req)
{
    return;
}