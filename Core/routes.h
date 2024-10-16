#pragma once
#ifndef _ROUTES
#define _ROUTES

#include<WinSock2.h>

enum METHODS {
    GET=0,
    POST=1,
    PUT=2
};

typedef struct Route {
    enum METHODS* methods;
    char* path;
    char* file;
    void (*get)(SOCKET *cscok);
} Route;

// Define Route Hash Tables

void getIndex(SOCKET *csock);
void get404(SOCKET *csock);
void defineRoute(enum METHODS method, char* path, char* html, void (*get)(SOCKET *cscok));

#endif