#pragma once
#ifndef _REQUEST_HANDLER

#include"Core/httpParser.h"
#include"Core/RoutingTable.h"

void routeRequest(SOCKET* csock, request req);

#endif