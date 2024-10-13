#ifndef _SERVER
#define _SERVER

#define DEFAULT_PORT 8080
#define BUFFER_SIZE 4096

void initialize(SOCKET* s);
void slisten(SOCKET* lsock, SOCKET* csock);
void handle();
void serve();
void gshutdown(SOCKET lsock);

#endif