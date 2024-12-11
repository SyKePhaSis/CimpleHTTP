#include <WinSock2.h>
#include "Core/routes.h"
#include "Core/httpParser.h"
#include "Core/RoutingTable.h"
#include "Utils/logger.h"

void routeRequest(SOCKET *csock, request req)
{
    RouteTable *rt = getRoutingTable();
    rfunc func = getRouteFunc(req, rt);
    if (func == NULL)
    {
        logError("Couldn't get route from RouteTable");
        return;
    }
    func(csock, req);
    deallocateReq(&req);
    return;
}
