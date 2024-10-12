#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

class SandRecvAll
{
public:
	static int sand_all(SOCKET s, char* buf, int len, int flags);
	static int recv_all(SOCKET s, char* buf, int len, int flags);
};

