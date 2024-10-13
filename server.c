#include<winsock2.h>
#include<stdio.h>
#include<WS2tcpip.h>

#include "Utils/httpParser.h"
#include "Utils/logger.h"
#include "server.h"

#pragma comment(lib,"Ws2_32.lib")

int main()
{   
    SOCKET lsock = INVALID_SOCKET;
    SOCKET csock = INVALID_SOCKET;
    initialize(&lsock);
    slisten(&lsock, &csock);
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
    if((*lsock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
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
    exit(EXIT_SUCCESS);
};

void slisten(SOCKET* lsock, SOCKET* csock){
    if ( listen( *lsock, SOMAXCONN ) == SOCKET_ERROR ) {
        logError( "Listen failed with error: %ld\n", WSAGetLastError() );
        gshutdown(*lsock);
    }

    char buffer[BUFFER_SIZE];
    while(1)
    {
        *csock = accept(*lsock, NULL, NULL);

        if (*csock == INVALID_SOCKET) {
            logError("Accept failed: %d\n", WSAGetLastError());
            gshutdown(*lsock);
        } 

        logSuccess("Made the Connection");
        recv(*csock, buffer, BUFFER_SIZE, 0);
        printf("%s", buffer);
    }
};

void handle(){};
void serve(){};
