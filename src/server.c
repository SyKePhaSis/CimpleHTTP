#include <WinSock2.h>
#include <stdio.h>
#include <WS2tcpip.h>
#include <string.h>
#include <stdlib.h>

#include "Core/httpParser.h"
#include "Core/routes.h"
#include "Core/requestHandler.h"
#include "Core/assetRouting.h"
#include "Core/handlerManager.h"
#include "Core/fileWatcher.h"
#include "Utils/logger.h"
#include "Utils/fileHandling.h"
#include "Utils/httpCreator.h"
#include "Utils/dotenv.h"
#include "Utils/memmoryTable.h"
#include "Utils/memmory.h"
#include "server.h"

void startServer()
{
    SOCKET lsock = INVALID_SOCKET;
    initialize(&lsock);
    slisten(&lsock);
    gshutdown(lsock);
}

void initialize(SOCKET *lsock)
{
    WSADATA wsa;
    struct sockaddr_in server;

    logInfo("Initialising Winsock...");
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        logError("Failed with error code: %d", WSAGetLastError());
        gshutdown(*lsock);
    }

    logInfo("Initialized Winsock");
    if ((*lsock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
    {
        logError("Couldn't create a socket: %d", WSAGetLastError());
        gshutdown(*lsock);
    }

    logSuccess("Socket Created");
    logInfo("Binding socket to port");

    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = htons(atoi((const char *)getEnvValue("PORT")));

    if (bind(*lsock, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
    {
        logError("Bind failed with error code : %d", WSAGetLastError());
        gshutdown(*lsock);
    }

    logSuccess("Socket Binded to Port: %s", getEnvValue("PORT"));
}

void gshutdown(SOCKET lsock)
{
    closesocket(lsock);
    WSACleanup();
}

void slisten(SOCKET *lsock)
{

    WatcherObject *handlerWatcher = createHandlerWatcher();

    if (listen(*lsock, SOMAXCONN) == SOCKET_ERROR)
    {
        logError("Listen failed with error: %ld\n", WSAGetLastError());
        gshutdown(*lsock);
    }

    logSuccess("Waiting for connection: http://localhost:%s", getEnvValue("PORT"));

    char buffer[BUFFER_SIZE];
    while (1)
    {
        /*

            CHECKING FOR HANDLER UPDATES

        */

        if (handlerWatcher)
        {
            retCheckWatcher rcw = checkForUpdate(handlerWatcher);
            if (rcw.updated)
            {
                logSuccess("Update Identified - Reload");
                reloadHandler(rcw.fileName);
            }
        }

        /*

            CONTINUING NETWORK LOOP

        */

        SOCKET csock = INVALID_SOCKET;
        struct sockaddr_in SenderAddr;
        int SenderAddrSize = sizeof(SenderAddr);

        csock = accept(*lsock, NULL, NULL);

        if (csock == INVALID_SOCKET)
        {
            logError("Accept failed: %d\n", WSAGetLastError());
            gshutdown(*lsock);
            continue;
        }
        logInfo("Made the Connection");
        int bytesReceived = recvfrom(csock, buffer, BUFFER_SIZE - 1, 0, (SOCKADDR *)&SenderAddr, &SenderAddrSize);
        char ip[17];
        if (inet_ntop(AF_INET, &SenderAddr.sin_addr, ip, 17) == NULL)
        {
            logError("Couldn't get Ip: %d\n", WSAGetLastError());
        }
        logInfo("Connection from Ip: %s", ip);
        logInfo("Received %d bytes", bytesReceived);
        if (bytesReceived > 0)
        {
            buffer[bytesReceived] = '\0';
        }
        handle(buffer, &csock);
        if (shutdown(csock, SD_BOTH) == SOCKET_ERROR)
        {
            logError("Couldn't lock READ & WRITE for the socket: %d", WSAGetLastError());
        }
        logInfo("Shutdown Socket");
        logInfo("Socket: %llu", csock);
        if (csock != INVALID_SOCKET)
        {
            closesocket(csock);
            csock = INVALID_SOCKET; // Set to invalid after closing
        }
        else
        {
            logInfo("Closed Socket");
        }
    }
}

void handle(char *req, SOCKET *csock)
{
    request reqi = extractRequestInfo(req);
    routeRequest(csock, reqi);
    logInfo("Had %lu allocations not freed", getAllocations());
    return;
}
