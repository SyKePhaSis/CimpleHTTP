#include "Core/routes.h"
#include "Core/dataHandling.h"
#include "Core/httpParser.h"
#include "Core/RoutingTable.h"
#include "Core/built_in_handlers.h"
#include "Utils/httpCreator.h"
#include "Utils/fileHandling.h"
#include "Utils/logger.h"
#include "Utils/memmory.h"
#include "DataTypes/dynamicArray.h"
#include "DataTypes/types.h"

#include <WinSock2.h>
#include <stdlib.h>

Array rt;

void getCssAsset(SOCKET *csock, request req)
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
            resBuffer buf = flushHttpRes(&res);
            sendData(csock, buf.buffer, buf.size);
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

void getJsAsset(SOCKET *csock, request req)
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
            resBuffer buf = flushHttpRes(&res);
            sendData(csock, buf.buffer, buf.size);
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

void getFontAsset(SOCKET *csock, request req)
{
    char *name = getFileName(req.path);
    char type[30] = "Content-Type: font/";
    if (name != NULL)
    {
        FileResp fr = getFile(name);
        if (fr.data != NULL || !fr.found)
        {
            httpResponse res = getHttpReq();
            addVersion(&res, "HTTP/1.1");
            addResCode(&res, "200");
            addMethod(&res, "OK");
            char *ext = getFileExt(req.path);
            strcat(type, ext);
            addHeader(&res, type);
            addHeader(&res, "Accept-ranges: bytes");
            addBody(&res, &fr);
            resBuffer buf = flushHttpRes(&res);
            sendData(csock, buf.buffer, buf.size);
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

void getImageAsset(SOCKET *csock, request req)
{
    char *name = getFileName(req.path);
    char type[30] = "Content-Type: image/";
    if (name != NULL)
    {
        FileResp fr = getFile(name);
        if (fr.data != NULL || !fr.found)
        {
            httpResponse res = getHttpReq();
            addVersion(&res, "HTTP/1.1");
            addResCode(&res, "200");
            addMethod(&res, "OK");
            char *ext = getFileExt(req.path);
            strcat(type, ext);
            addHeader(&res, type);
            addBody(&res, &fr);
            resBuffer buf = flushHttpRes(&res);
            sendData(csock, buf.buffer, buf.size);
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
    if (fr.found && fr.data != NULL)
    {
        httpResponse res = getHttpReq();
        addVersion(&res, "HTTP/1.1");
        addResCode(&res, "404");
        addMethod(&res, "Not Found");
        addHeader(&res, "Content-Type: text/html");
        addBody(&res, &fr);
        resBuffer buf = flushHttpRes(&res);
        sendData(csock, buf.buffer, buf.size);
        logConnection("%s %s 404 Not Found", req.version, req.path);
    }
    else
    {
        logError("FileResp returned with NULL data");
        built_in_internal_server_error(csock, req);
    }
}

void defineRoute(enum METHODS method, char *path, void (*func)(SOCKET *csock, request req))
{
    Route *r = allocate(sizeof(Route));
    r->method = method;
    r->path = path;
    r->allocated = 0;
    r->func = func;
    addToArray(&rt, r);
}

void defineRouter(Router r)
{
    if (r.get)
        defineRoute(GET, r.path, r.get);
    if (r.post)
        defineRoute(POST, r.path, r.post);
    if (r.put)
        defineRoute(PUT, r.path, r.put);
    if (r.delete)
        defineRoute(DEL, r.path, r.delete);
}

void defineAssetRoute(char *path, enum ASSET_TYPE asset)
{
    Route *r = allocate(sizeof(Route));
    r->method = GET;
    r->path = path;
    r->func = getFunctionForAsset(asset);
    r->allocated = 1;
    addToArray(&rt, r);
}

rFunc getFunctionForAsset(enum ASSET_TYPE asset)
{
    if (asset == CSS)
    {
        return getCssAsset;
    }
    else if (asset == JS)
    {
        return getJsAsset;
    }
    else if (asset == FONT)
    {
        return getFontAsset;
    }
    else if (asset == IMAGE)
    {
        return getImageAsset;
    }
    return NULL;
}

Array *getRoutingTable()
{
    return &rt;
}

void initRoutingTable()
{
    rt = initializeArray(ROUTER_ARR);
}

// Helper Functions

void printRoutes()
{
    Iterator *it = createIterator(&rt);
    while (iteratorHasNext(it))
    {
        Route *r = iteratorGetNext(it);
        char *meth = "";
        switch (r->method)
        {
        case GET:
            meth = "GET";
            break;

        case POST:
            meth = "POST";
            break;

        case PUT:
            meth = "PUT";
            break;

        case DEL:
            meth = "DEL";
            break;

        case UNKNOWN:
            meth = "UNKNOWN";
            break;
        }
        logSuccess("Route [%s] %s", meth, r->path);
    }
}
