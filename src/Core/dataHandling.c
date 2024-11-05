#include "Core/dataHandling.h"
#include "Utils/logger.h"
#include "Utils/memmory.h"
#include <WinSock2.h>
#include <stdlib.h>

int sendData(SOCKET *csock, void *buf, size_t size)
{
    size_t bytes_sent = send(*csock, buf, size, 0);
    if (bytes_sent == (size_t)SOCKET_ERROR)
    {
        logError("Coudln't send Response.");
        closesocket(*csock);
        deallocate(buf);
        return 0;
    }
    else if (bytes_sent != size)
    {
        logError("Didn't send all data: sent %d", bytes_sent);
        deallocate(buf);
        return 0;
    }
    else
    {
        logInfo("Sent %d bytes", bytes_sent);
        logInfo("Successfully sent response");
        deallocate(buf);
        return 1;
    }
}
