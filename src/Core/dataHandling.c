#include "Core/dataHandling.h"
#include "Utils/logger.h"
#include "Utils/memmory.h"
#include <WinSock2.h>
#include <stdlib.h>

int sendData(SOCKET *csock, void *buf, size_t size)
{
    int ret = 0;
    size_t bytes_sent = send(*csock, buf, size, 0);
    if (bytes_sent == (size_t)SOCKET_ERROR)
    {
        logError("Coudln't send Response.");
        closesocket(*csock);
    }
    else if (bytes_sent != size)
    {
        logError("Didn't send all data: sent %d", bytes_sent);
    }
    else
    {
        logInfo("Sent %d bytes", bytes_sent);
        logInfo("Successfully sent response");
        ret = 1;
    }

    deallocate(buf);
    return ret;
}
