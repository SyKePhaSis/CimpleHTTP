#include <string.h>
#include "Core/RoutingTable.h"
#include "Core/built_in_handlers.h"
#include "Utils/logger.h"

void addToRouteTable(Route *r, RouteTable *rt)
{
    if (rt->count == 100)
    {
        logError("Maximum Routes allowed reached");
    }
    else
    {
        rt->count++;
    }
    rt->routes[rt->count - 1] = r;
}

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

rfunc getRouteFunc(request req, RouteTable *r)
{
    enum METHODS method = getMethodFromString(req.method);
    if (method != UNKNOWN)
    {
        for (size_t i = 0; i < r->count; i++)
        {
            if (strcmp(r->routes[i]->path, req.path) == 0 && r->routes[i]->method == method)
            {
                return r->routes[i]->func;
            }
        }
    }
    if (method == GET)
    {
        // Find * route
        for (size_t i = 0; i < r->count; i++)
        {
            if (strcmp(r->routes[i]->path, "*") == 0)
            {
                logInfo("Couldn't find route returned *");
                return r->routes[i]->func;
            }
        }
        return NULL;
    }
    else
    {
        return built_in_unsuported_method;
    }
    return built_in_internal_server_error;
}