#ifndef _SERVER
#define _SERVER

#define DEFAULT_PORT 8080
#define BUFFER_SIZE 4096

// #pragma comment(lib,"Ws2_32.lib")

void initialize(SOCKET *s);
void slisten(SOCKET *lsock);
void handle(char *req, SOCKET *csock);
void serve(request req, SOCKET *csock);
void gshutdown(SOCKET lsock);

#endif