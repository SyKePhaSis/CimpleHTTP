#ifndef _SERVER
#define _SERVER

#define DEFAULT_PORT 8080
#define BUFFER_SIZE 4096

// #pragma comment(lib,"Ws2_32.lib")
#include <WinSock2.h>

void initialize(SOCKET *s);
void slisten(SOCKET *lsock);
void handle(char *req, SOCKET *csock);
void gshutdown(SOCKET lsock);
void startServer(void);

#endif