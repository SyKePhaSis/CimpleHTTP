#include"Utils/httpCreator.h"
#include"Utils/fileHandling.h"
#include"Utils/logger.h"
#include"Core/routes.h"
#include"Core/dataHandling.h"
#include"Core/httpParser.h"
#include"Core/RoutingTable.h"

#include<WinSock2.h>

RouteTable rt = {.count = 0};

void getIndex(SOCKET *csock, request req){
    FileResp fr = getFile("/index.html");
    if(fr.data != NULL)
    {
        httpResponse res = getHttpReq();
        addVersion(&res, "HTTP/1.1");
        addResCode(&res, "200");
        addMethod(&res, "OK");
        addHeader(&res,"Content-Type: text/html");
        addBody(&res, &fr);
        char* buf = flushHttpRes(&res);
        sendData(csock, buf);
        logConnection("%s %s 200 OK", req.version, req.path);
    } else {
        logError("FileResp returned with NULL data");
        get404(csock, req);
    }
}

void get404(SOCKET *csock, request req){
    FileResp fr = getFile("/404.html");
    if(fr.data != NULL)
    {
        httpResponse res = getHttpReq();
        addVersion(&res, "HTTP/1.1");
        addResCode(&res, "404");
        addMethod(&res, "Not Found");
        addHeader(&res,"Content-Type: text/html");
        addBody(&res, &fr);
        char* buf = flushHttpRes(&res);
        sendData(csock, buf);
        logConnection("%s %s 404 Not Found", req.version, req.path);
    } else {
        logError("FileResp returned with NULL data");
    }
}

void defineRoute(enum METHODS method, char* path ,void (*func)(SOCKET *cscok, request req))
{
    Route* r = malloc(sizeof(Route));
    r->method = method;
    r->path = path;
    r->func = func;
    addToRouteTable(r, &rt);
}

//Helper Function

RouteTable* getRoutingTable()
{
    return &rt;
}
