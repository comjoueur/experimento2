#ifndef SERCICIO_RED_H_INCLUDED
#define SERCICIO_RED_H_INCLUDED

#include <winsock2.h>
#include <Windows.h>
int send(
  _In_       SOCKET s,
  _In_ const char   *buf,
  _In_       int    len,
  _In_       int    flags
);

class NetworkServices
{

public:

static int sendMessage(SOCKET curSocket, char * message, int messageSize);
static int receiveMessage(SOCKET curSocket, char * buffer, int bufSize);

};

#endif // SERCICIO_RED_H_INCLUDED
