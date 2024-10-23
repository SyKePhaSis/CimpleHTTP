#include <WinSock2.h>
#include "Core/routes.h"
#include "Core/httpParser.h"
#include "Core/RoutingTable.h"
#include "Utils/logger.h"

void routeRequest(SOCKET *csock, request req)
{
    RouteTable *rt = getRoutingTable();
    Route *r = getRoute(req.path, rt);
    if (r == NULL)
    {
        logError("Couldn't get route from RouteTable");
        return;
    }
    r->func(csock, req);
    deallocateReq(&req);
    return;
}
