#ifndef _ROUTING_TABLE
#define _ROUTING_TABLE

#include <WinSock2.h>
#include "Core/httpParser.h"

enum METHODS
{
    GET = 0,
    POST = 1,
    PUT = 2,
    DEL = 3,
    UNKNOWN = -1
};

typedef void (*rfunc)(SOCKET *csock, request req);

typedef struct Route
{
    enum METHODS method;
    char *path;
    int allocated;
    rfunc func;
} Route;

typedef struct RouteTable
{
    Route *routes[100];
    size_t count;
} RouteTable;

void addToRouteTable(Route *r, RouteTable *rt);
rfunc getRouteFunc(request req, RouteTable *r);

#endif