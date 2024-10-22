#pragma once
#ifndef _ROUTES
#define _ROUTES

#include <WinSock2.h>
#include "Core/httpParser.h"
#include "Core/RoutingTable.h"

void getIndex(SOCKET *csock, request req);
void get404(SOCKET *csock, request req);
void getAsset(SOCKET *csock, request req);
void defineRoute(enum METHODS method, char *path, void (*func)(SOCKET *cscok, request req));
void defineAssetRoute(char *path);
void printRoutes(void);
RouteTable *getRoutingTable();

#endif