#include<WinSock2.h>
#include<stdio.h>
#include<WS2tcpip.h>
#include<string.h>
#include<stdlib.h>

#include "Core/httpParser.h"
#include "Core/routes.h"
#include "Utils/logger.h"
#include "Utils/fileHandling.h"
#include "Utils/httpCreator.h"
#include "server.h"

int main()
{   
    SOCKET lsock = INVALID_SOCKET;
    initialize(&lsock);
    slisten(&lsock);
    gshutdown(lsock);
}

void initialize(SOCKET* lsock)
{
    WSADATA wsa;
    struct sockaddr_in server;

    logInfo("Initialising Winsock...");
    if(WSAStartup(MAKEWORD(2,2), &wsa) != 0) 
    {
        logError("Failed with error code: %d", WSAGetLastError());
        gshutdown(*lsock);
    }

    logInfo("Initialized Winsock");
    if((*lsock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == INVALID_SOCKET)
    {
        logError("Couldn't create a socket: %d", WSAGetLastError());
        gshutdown(*lsock);
    }

    logSuccess("Socket Created");
    logInfo("Binding socket to port");
    

    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_family = AF_INET;
    server.sin_port = htons(DEFAULT_PORT);

    if( bind(*lsock , (struct sockaddr *)&server , sizeof(server)) == SOCKET_ERROR)
    {
        logError("Bind failed with error code : %d" , WSAGetLastError());
        gshutdown(*lsock);
    }

    logSuccess("Socket Binded to Port: %d", DEFAULT_PORT);
}

void gshutdown(SOCKET lsock){
    closesocket(lsock);
    WSACleanup();   
}

void slisten(SOCKET* lsock){

    if ( listen( *lsock, SOMAXCONN ) == SOCKET_ERROR ) {
        logError( "Listen failed with error: %ld\n", WSAGetLastError() );
        gshutdown(*lsock);
    }

    char buffer[BUFFER_SIZE];
    while(1)
    {
        SOCKET csock = INVALID_SOCKET;
        logInfo("Waiting for connection: http://localhost:%d", DEFAULT_PORT);
        csock = accept(*lsock, NULL, NULL);

        if (csock == INVALID_SOCKET) {
            logError("Accept failed: %d\n", WSAGetLastError());
            gshutdown(*lsock);
        } 
        logSuccess("Made the Connection");
        int bytesReceived = recv(csock, buffer, BUFFER_SIZE - 1, 0);
        logInfo("Received %d bytes", bytesReceived);
        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';
        }
        handle(buffer, &csock);
        if(shutdown(csock, SD_BOTH) == SOCKET_ERROR)
        {
            logError("Couldn't lock READ & WRITE for the socket: %d", WSAGetLastError());
        }
        logInfo("Shutdown Socket");
        logInfo("Socket: %llu", csock);
        if (csock != INVALID_SOCKET) {
            closesocket(csock);
            csock = INVALID_SOCKET;  // Set to invalid after closing
        } else {
            logInfo("Closed Socket");
        }
    }
}

void handle(char* req, SOCKET* csock){
    request reqi = extractRequestInfo(req);
    logInfo("%s %s %s", reqi.method, reqi.path);
    serve(reqi, csock);
    return;
}

void serve(request req, SOCKET* csock){
    if(strcmp(req.path, "/") == 0)
    {
        getIndex(csock);
    } else {
        get404(csock);
    }
    return;
}