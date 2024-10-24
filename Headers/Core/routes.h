#pragma once
#ifndef _ROUTES
#define _ROUTES

#include <WinSock2.h>
#include "Core/httpParser.h"
#include "Core/RoutingTable.h"

enum ASSET_TYPE
{
    CSS,
    JS,
    IMAGE,
    FONT
};

typedef void (*rFunc)(SOCKET *, request);

void getIndex(SOCKET *csock, request req);
void get404(SOCKET *csock, request req);
void getCssAsset(SOCKET *csock, request req);
void getJsAsset(SOCKET *csock, request req);
void getFontAsset(SOCKET *csock, request req);
void getImageAsset(SOCKET *csock, request req);
void defineRoute(enum METHODS method, char *path, void (*func)(SOCKET *cscok, request req));
void defineAssetRoute(char *path, enum ASSET_TYPE asset);
void printRoutes(void);
rFunc getFunctionForAsset(enum ASSET_TYPE asset);
RouteTable *getRoutingTable();

#endif