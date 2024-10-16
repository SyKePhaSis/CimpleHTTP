#pragma once
#ifndef httpParser
#define httpParser

#define _ARRAY_START_SIZE 30

typedef struct request {
    char* version;
    char* method;
    char* code;
    char* path;
    char* params;
    int valid;
    short int failed;
} request;

typedef struct sreq {
    char** s_arr;
    int len;
    int finished;
} sreq;

// char* getPath(char* req);
// char* getQueryParams(char* req);
// int verifyRequest(char* req);
sreq splitReq(char* req);
request extractRequestInfo(char* req);
void getInfo(request* req, char* line);
void printSreq(sreq req);
void freeSREQ(sreq split_req);

#endif