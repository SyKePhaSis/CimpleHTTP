#include "Core/returnReqs.h"
#include "Core/httpParser.h"
#include "Core/routes.h"
#include "Core/dataHandling.h"
#include "Utils/fileHandling.h"
#include "Utils/httpCreator.h"
#include "Utils/logger.h"
#include <WinSock2.h>

void view_or_404(SOCKET *s, request req, char *path)
{
    FileResp fr = getFile(path);
    if (fr.data != NULL || !fr.found)
    {
        httpResponse res = getHttpReq();
        addVersion(&res, "HTTP/1.1");
        addResCode(&res, "200");
        addMethod(&res, "OK");
        addHeader(&res, "Content-Type: text/html");
        addBody(&res, &fr);
        resBuffer buf = flushHttpRes(&res);
        sendData(s, buf.buffer, buf.size);
        logConnection("%s %s 200 OK", req.version, req.path);
    }
    else
    {
        logError("FileResp returned with NULL data");
        view_404(s, req);
    }
}

void view_404(SOCKET *s, request req)
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
        resBuffer buf = flushHttpRes(&res);
        sendData(s, buf.buffer, buf.size);
        logConnection("%s %s 404 Not Found", req.version, req.path);
    }
    else
    {
        logError("Couldn't find 404 file");
    }
}

void json_200(SOCKET *s, request req, char *json)
{
    httpResponse res = getHttpReq();
    addVersion(&res, "HTTP/1.1");
    addResCode(&res, "200");
    addMethod(&res, "OK");
    addHeader(&res, "Content-Type: text/json");
    addBodyText(&res, json);
    resBuffer buf = flushHttpRes(&res);
    sendData(s, buf.buffer, buf.size);
    logConnection("%s %s 200 OK", req.version, req.path);
}