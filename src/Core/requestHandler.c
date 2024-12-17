#include <WinSock2.h>
#include "Core/routes.h"
#include "Core/httpParser.h"
#include "Core/RoutingTable.h"
#include "Utils/memmory.h"
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
    int alocp = getAllocations();
    func(csock, req);
    int aloca = getAllocations();
    if (aloca - alocp > 0)
    {
        logWarning("Non freed allocations found, please optimize the code: %d allocations not freed", aloca - alocp);
    }
    deallocateReq(&req);
    return;
}
