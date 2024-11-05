#include <string.h>
#include "Core/RoutingTable.h"
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

Route *getRoute(char *path, RouteTable *r)
{
    for (size_t i = 0; i < r->count; i++)
    {
        if (strcmp(r->routes[i]->path, path) == 0)
        {
            return r->routes[i];
        }
    }

    // Find * route
    for (size_t i = 0; i < r->count; i++)
    {
        if (strcmp(r->routes[i]->path, "*") == 0)
        {
            return r->routes[i];
        }
    }
    return NULL;
}