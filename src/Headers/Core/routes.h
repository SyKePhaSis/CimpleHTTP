#ifndef _ROUTES
#define _ROUTES

#include <WinSock2.h>
#include "Core/httpParser.h"
#include "Core/RoutingTable.h"

typedef void (*rFunc)(SOCKET *, request);

enum ASSET_TYPE
{
    CSS,
    JS,
    IMAGE,
    FONT
};

enum RouteState
{
    ENABLED = 1,
    DISABLED = 0
};

typedef struct Router
{
    char *path;
    rFunc get;
    rFunc post;
    rFunc put;
    rFunc delete;
} Router;

typedef struct RouterConfig
{
    char *path;
    enum RouteState get;
    enum RouteState post;
    enum RouteState put;
    enum RouteState delete;
} RouterConfig;

void getIndex(SOCKET *csock, request req);
void get404(SOCKET *csock, request req);
void getCssAsset(SOCKET *csock, request req);
void getJsAsset(SOCKET *csock, request req);
void getFontAsset(SOCKET *csock, request req);
void getImageAsset(SOCKET *csock, request req);
void defineRoute(enum METHODS method, char *path, void (*func)(SOCKET *csock, request req));
void defineRouter(Router r);
void defineAssetRoute(char *path, enum ASSET_TYPE asset);
void printRoutes(void);
void initRoutingTable();
rFunc getFunctionForAsset(enum ASSET_TYPE asset);
Array *getRoutingTable();

#endif