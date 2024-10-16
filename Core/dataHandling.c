#include "dataHandling.h"
#include "../Utils/logger.h"
#include<WinSock2.h>
#include<stdlib.h>

int sendData(SOCKET *csock, char* buf)
{
    int bytes_sent = send(*csock, buf, strlen(buf)*sizeof(char), 0);
    if(bytes_sent == SOCKET_ERROR)
    {
        logError("Coudln't send Response.");
        return 0;
        closesocket(*csock);
    } else if (bytes_sent != strlen(buf)) {
        logError("Didn't send all data: sent %d",  bytes_sent);
        return 0;
    } else {
        logInfo("Sent %d bytes", bytes_sent);
        logSuccess("Successfully sent response");
        return 1;
    }
    free(buf);
}
        