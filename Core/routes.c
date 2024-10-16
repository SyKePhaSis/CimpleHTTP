#include"routes.h"
#include"../Utils/httpCreator.h"
#include"../Utils/fileHandling.h"
#include"../Utils/logger.h"

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
        char* sres = flushHttpRes(&res);
        int bytes_sent = send(*csock, sres, strlen(sres)*sizeof(char), 0);
        if(bytes_sent == SOCKET_ERROR)
        {
            logError("Coudln't send Response.");
            closesocket(*csock);
        } else if (bytes_sent != strlen(sres)) {
            logInfo("Didn't send all data: sent %d",  bytes_sent);
        } else {
            logInfo("Sent %d bytes", bytes_sent);
            logSuccess("Successfully sent response");
        }
        free(sres);
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
        addResCode(&res, "200");
        addMethod(&res, "OK");
        addHeader(&res,"Content-Type: text/html");
        addBody(&res, &fr);
        char* buf = flushHttpRes(&res);
        if(send(*csock, buf, strlen(buf)*sizeof(char), 0) < 0)
        {
            logError("Coudln't send Response.");
        } 
    } else {
        logError("FileResp returned with NULL data");
    }
}