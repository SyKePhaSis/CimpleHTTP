#pragma once
#ifndef _ROUTING_TABLE
#define _ROUTING_TABLE

#include <WinSock2.h>
#include "Core/httpParser.h"

enum METHODS
{
    GET = 0,
    POST = 1,
    PUT = 2
};

typedef struct Route
{
    enum METHODS method;
    char *path;
    int allocated;
    void (*func)(SOCKET *cscok, request req);
} Route;

typedef struct RouteTable
{
    Route *routes[100];
    size_t count;
} RouteTable;

void addToRouteTable(Route *r, RouteTable *rt);
Route *getRoute(char *path, RouteTable *r);

#endif