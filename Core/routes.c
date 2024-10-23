#include "Core/routes.h"
#include "Core/dataHandling.h"
#include "Core/httpParser.h"
#include "Core/RoutingTable.h"
#include "Utils/httpCreator.h"
#include "Utils/fileHandling.h"
#include "Utils/logger.h"
#include "Utils/memmory.h"

#include <WinSock2.h>
#include <stdlib.h>

RouteTable rt = {.count = 0};

void getIndex(SOCKET *csock, request req)
{
    FileResp fr = getFile("index.html");
    if (fr.data != NULL || !fr.found)
    {
        httpResponse res = getHttpReq();
        addVersion(&res, "HTTP/1.1");
        addResCode(&res, "200");
        addMethod(&res, "OK");
        addHeader(&res, "Content-Type: text/html");
        addBody(&res, &fr);
        char *buf = flushHttpRes(&res);
        sendData(csock, buf);
        logConnection("%s %s 200 OK", req.version, req.path);
    }
    else
    {
        logError("FileResp returned with NULL data");
        get404(csock, req);
    }
}

void getCSSAsset(SOCKET *csock, request req)
{
    char *name = getFileName(req.path);
    if (name != NULL)
    {
        FileResp fr = getFile(name);
        if (fr.data != NULL || !fr.found)
        {
            httpResponse res = getHttpReq();
            addVersion(&res, "HTTP/1.1");
            addResCode(&res, "200");
            addMethod(&res, "OK");
            addHeader(&res, "Content-Type: text/css");
            addBody(&res, &fr);
            char *buf = flushHttpRes(&res);
            sendData(csock, buf);
            logConnection("%s %s 200 OK", req.version, req.path);
        }
        else
        {
            logError("FileResp returned with NULL data");
            get404(csock, req);
        }
    }
    else
    {
        get404(csock, req);
    }
}

void getJSAsset(SOCKET *csock, request req)
{
    char *name = getFileName(req.path);
    if (name != NULL)
    {
        FileResp fr = getFile(name);
        if (fr.data != NULL || !fr.found)
        {
            httpResponse res = getHttpReq();
            addVersion(&res, "HTTP/1.1");
            addResCode(&res, "200");
            addMethod(&res, "OK");
            addHeader(&res, "Content-Type: text/javascript");
            addBody(&res, &fr);
            char *buf = flushHttpRes(&res);
            sendData(csock, buf);
            logConnection("%s %s 200 OK", req.version, req.path);
        }
        else
        {
            logError("FileResp returned with NULL data");
            get404(csock, req);
        }
    }
    else
    {
        get404(csock, req);
    }
}

void get404(SOCKET *csock, request req)
{
    FileResp fr = getFile("404.html");
    if (fr.data != NULL)
    {
        httpResponse res = getHttpReq();
        addVersion(&res, "HTTP/1.1");
        addResCode(&res, "404");
        addMethod(&res, "Not Found");
        addHeader(&res, "Content-Type: text/html");
        addBody(&res, &fr);
        char *buf = flushHttpRes(&res);
        sendData(csock, buf);
        logConnection("%s %s 404 Not Found", req.version, req.path);
    }
    else
    {
        logError("FileResp returned with NULL data");
    }
}

void defineRoute(enum METHODS method, char *path, void (*func)(SOCKET *cscok, request req))
{
    Route *r = allocate(sizeof(Route));
    r->method = method;
    r->path = path;
    r->allocated = 0;
    r->func = func;
    addToRouteTable(r, &rt);
}

void defineAssetRoute(char *path, enum ASSET_TYPE asset)
{
    Route *r = allocate(sizeof(Route));
    r->method = GET;
    r->path = path;
    if (asset == CSS)
    {
        r->func = getCSSAsset;
    }
    r->allocated = 1;
    addToRouteTable(r, &rt);
}

// Helper Function

RouteTable *getRoutingTable()
{
    return &rt;
}

void printRoutes()
{
    for (size_t i = 0; i < rt.count; i++)
    {
        logSuccess("Route %s", rt.routes[i]->path);
    }
}
