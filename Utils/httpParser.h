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
} request;

typedef struct sreq {
    char** s_arr;
    int len;
} sreq;

request getMethod(char* request);
char* getPath(char* request);
char* getQueryParams(char* request);
sreq splitReq(char* request);
int verifyRequest(char* request);
request extractRequestInfo(char* request);
void printSreq(sreq req);

#endif