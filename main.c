#include<winsock2.h>
#include<stdio.h>
#include <WS2tcpip.h>

#define SRV_PORT 80
#define SRV_IP "142.250.187.142"
#define CLNT_BUFF 4096

#pragma comment(lib,"Ws2_32.lib")

int main()
{   
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;
    char* message, server_reply[CLNT_BUFF];
    int recv_size;

    printf("[INFO] Initialising Winsock...\n");
    if(WSAStartup(MAKEWORD(2,2), &wsa) != 0) 
    {
        printf("[ERROR] Failed with error code: %d", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    printf("[INFO] Initialized Winsock\n");

    if((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        printf("[ERROR] Couldn't create a socket: %d\n", WSAGetLastError());
        exit(EXIT_FAILURE);
    }

    printf("[INFO] Socket Created\n");
    printf("[INFO] Server IP: %s\n", SRV_IP);
    printf("[INFO] Server PORT: %d\n", SRV_PORT);

    server.sin_addr.s_addr = inet_addr(SRV_IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(SRV_PORT);

    if(connect(s, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        printf("[ERROR] Connection error");
        exit(EXIT_FAILURE);
    }

    printf("[INFO] Connected\n");
    message = "GET / HTTP/1.1\r\n\r\n";
    if( send(s, message,strlen(message),0) < 0)
    {
        printf("[ERROR] Sending data failed!");
        exit(EXIT_FAILURE);
    }

    printf("[INFO] Send %llu Bytes\n", strlen(message));

    if((recv_size = recv(s, server_reply, CLNT_BUFF, 0)) == SOCKET_ERROR) {
        printf("[ERROR] Couldn't receive the message");
        exit(EXIT_FAILURE);
    }

    printf("[INFO] Received %d bytes in response.\n", recv_size);

    server_reply[recv_size] = 0x00;

    printf("[INFO] The reply was: \n %s", server_reply);

    return 0;
}