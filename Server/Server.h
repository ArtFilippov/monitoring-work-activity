#pragma once
#include <iostream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include <map>
#include <vector>
#include <mutex>
#include <thread>
#include <memory>

#include "TalkToClient.h"
#include "View.h"

#pragma comment(lib, "Ws2_32.lib")

class Server
{
	WSADATA wsData{};
	SOCKET Sock;

	std::vector<std::string> keys{};
	std::map<std::string, std::shared_ptr<TalkToClient>> clients;
	std::mutex m;

	std::thread t;

	View view;
public:
	Server();
	~Server();

private:
	void new_client(std::shared_ptr<TalkToClient> client);
	void get_screenshot(int num);
	void run();
	void start_listen();
};