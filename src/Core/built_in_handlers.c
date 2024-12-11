#include <WinSock2.h>
#include "Utils/httpCreator.h"
#include "Utils/logger.h"
#include "Core/dataHandling.h"
#include "Core/built_in_handlers.h"
#include "Core/httpParser.h"

void built_in_unsuported_method(SOCKET *csock, request req)
{
    httpResponse res = getHttpReq();
    addVersion(&res, "HTTP/1.1");
    addResCode(&res, "405");
    addMethod(&res, "Method Not Allowed");
    addBodyEmpty(&res);
    resBuffer buf = flushHttpRes(&res);
    sendData(csock, buf.buffer, buf.size);
    logConnection("%s %s 405 Method Not Allowed", req.version, req.path);
}

void built_in_internal_server_error(SOCKET *csock, request req)
{
    httpResponse res = getHttpReq();
    addVersion(&res, "HTTP/1.1");
    addResCode(&res, "500");
    addMethod(&res, "Internal Server Error");
    addBodyEmpty(&res);
    resBuffer buf = flushHttpRes(&res);
    sendData(csock, buf.buffer, buf.size);
    logConnection("%s %s 500 Internal Server Error", req.version, req.path);
}