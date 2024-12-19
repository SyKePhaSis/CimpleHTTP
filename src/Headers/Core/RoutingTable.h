#ifndef _ROUTING_TABLE
#define _ROUTING_TABLE

#include <WinSock2.h>
#include "Core/httpParser.h"
#include "DataTypes/types.h"

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

rfunc getRouteFunc(request req, Array *r);

#endif