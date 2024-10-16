#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#include"httpParser.h"
#include"../Utils/logger.h"

sreq splitReq(char* req)
{
    sreq resp;
    resp.finished = 0;
    char **out = malloc(sizeof(char*));
    if(out == NULL)
    {
        logError("Couldn't allocate memory to save request");
        return resp;
    }

    char *line;
    int c = 0;
    line = strtok(req, "\r\n");
    while(line != NULL)
    {
        c += 1;
        out = realloc(out, c * sizeof(char*));
        if(out == NULL)
        {
            logError("Couldn't append allocated memory to save request.");
            return resp;
        }

        out[c-1] = malloc(sizeof(char) * (strlen(line) + 1));
        if(out[c-1] == NULL)
        {
            logError("Couldn't Allocate Memory to save Line");
            return resp;
        }
        strcpy(out[c-1], line);
        
        line = strtok(NULL, "\r\n");
    }
    resp.finished = 1;
    resp.s_arr = out;
    resp.len = c;
    return resp;
}

void getInfo(request* req, char* line)
{
    char* fs[3] = {0};
    char* tok;
    int c = 0;
    tok = strtok(line, " ");
    while(tok != NULL)
    {
        if(c < 3) {
            fs[c] = malloc((strlen(tok) + 1)*sizeof(char));
            strcpy(fs[c], tok);
        } else {
            logError("Needed more space to store Method");
        }
        c++;
        tok = strtok(NULL, " ");
    }
    req->method = fs[0];
    req->path = fs[1];
    req->version = fs[2];
}

// char* getPath(char* req){return NULL;}
// char* getQueryParams(char* req){return NULL;}
// int verifyRequest(char* req){return 0;}

request extractRequestInfo(char* req)
{
    request ret;
    ret.failed = 0;
    sreq split_req = splitReq(req);
    if(!split_req.finished)
    {
        return ret;
    }
    getInfo(&ret, split_req.s_arr[0]);
    return ret;
}

// Helpers
void printSreq(sreq req)
{
    for(int i = 0; i < req.len; i++)
    {
        printf("%d: %s \n", i,*(req.s_arr + i));
    }
    return;
}