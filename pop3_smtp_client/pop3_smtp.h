#include <WinSock2.h>

char* sendRequest(SOCKET s,char* req,char* buf);
SOCKET conn(char* mail, int port);
char* getLogin(char* addr);
