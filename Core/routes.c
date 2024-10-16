#include"routes.h"
#include"../Utils/httpCreator.h"
#include"../Utils/fileHandling.h"
#include"../Utils/logger.h"
#include"../Core/dataHandling.h"

#include<WinSock2.h>

void getIndex(SOCKET *csock){
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
    } else {
        logError("FileResp returned with NULL data");
    }
}

void get404(SOCKET *csock){
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
    } else {
        logError("FileResp returned with NULL data");
    }
}

void defineRoute(enum METHODS method, char* path, char* html, void (*get)(SOCKET *cscok))
{

};