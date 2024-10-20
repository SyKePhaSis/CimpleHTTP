#pragma once
#ifndef _ROUTES
#define _ROUTES

#include<WinSock2.h>
#include "../Core/httpParser.h"

enum METHODS {
    GET=0,
    POST=1,
    PUT=2
};

typedef struct Route {
    enum METHODS method;
    char* path;
    void (*func)(SOCKET *cscok, request req);
} Route;

typedef struct RouteTable {
    Route* routes[100];
    size_t count;
} RouteTable;

void getIndex(SOCKET *csock, request req);
void get404(SOCKET *csock, request req);
void defineRoute(enum METHODS method, char* path, void (*get)(SOCKET *cscok, request req));
void addToRouteTable(Route* r);

#endif