#include <string.h>
#include "Core/RoutingTable.h"
#include "Core/built_in_handlers.h"
#include "DataTypes/dynamicArray.h"
#include "DataTypes/types.h"
#include "Utils/logger.h"

enum METHODS getMethodFromString(char *method)
{
    if (strcmp(method, "GET") == 0)
    {
        return GET;
    }
    else if (strcmp(method, "POST") == 0)
    {
        return POST;
    }
    else if (strcmp(method, "PUT") == 0)
    {
        return PUT;
    }
    else if (strcmp(method, "DELETE") == 0)
    {
        return DEL;
    }
    return UNKNOWN;
}

rfunc getRouteFunc(request req, Array *rt)
{
    enum METHODS method = getMethodFromString(req.method);
    if (method != UNKNOWN)
    {
        Iterator *it = createIterator(rt);
        while (iteratorHasNext(it))
        {
            Route *r = iteratorGetNext(it);
            if (strcmp(r->path, req.path) == 0 && r->method == method)
                return r->func;
        }
        iteratorCleanup(it);
    }
    if (method == GET)
    {
        // Find * route
        Iterator *it = createIterator(rt);
        while (iteratorHasNext(it))
        {
            Route *r = iteratorGetNext(it);
            if (strcmp(r->path, "*") == 0 && r->method == method)
            {
                logInfo("Couldn't find Route returned /* ");
                return r->func;
            }
        }
        iteratorCleanup(it);
    }
    else
    {
        return built_in_unsuported_method;
    }
    return built_in_internal_server_error;
}