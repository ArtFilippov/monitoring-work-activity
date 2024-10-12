#include "Server.h"

Server::Server() : view(keys)
{
	int erStat = WSAStartup(MAKEWORD(2, 2), &wsData);

	if (erStat != 0) {
		std::cerr << "Error WinSock version initializaion" << std::endl;
		std::cerr << WSAGetLastError();
		throw std::string("Error WinSock version initializaion");
	}

	Sock = socket(AF_INET, SOCK_STREAM, 0);

	if (Sock == INVALID_SOCKET) {
		std::cerr << "Error initialization socket # " << WSAGetLastError() << std::endl;
		throw std::string("Error WinSock version initializaion");
	}

	in_addr ip_to_num;
	erStat = inet_pton(AF_INET, "127.0.0.1", &ip_to_num);
	if (erStat <= 0) {
		std::cerr << "Error in IP translation to special numeric format" << std::endl;
		throw std::string("Error in IP translation to special numeric format");
	}

	sockaddr_in servInfo;

	ZeroMemory(&servInfo, sizeof(servInfo));

	servInfo.sin_family = AF_INET;
	servInfo.sin_addr = ip_to_num;
	servInfo.sin_port = htons(1234);

	erStat = bind(Sock, (sockaddr*)&servInfo, sizeof(servInfo));

	if (erStat != 0) {
		std::cerr << "Error Socket binding to server info. Error # " << WSAGetLastError() << std::endl;
		
		throw std::string("Error Socket binding to server info. Error #");
	}

	run();
}

Server::~Server()
{
	closesocket(Sock);
	WSACleanup();
	t.join();
}

void Server::run()
{
	t = std::thread(&Server::start_listen, this);

	int num;
	while (std::cin >> num) {
		if (num >= 0 && num < (int)keys.size()) {
			get_screenshot(num);
		}
		else {
			view.update_screenshot_path("invalid input");
		}
	}

	std::cout << "goodbye!" << std::endl;
}

void Server::start_listen()
{
	int erStat = listen(Sock, SOMAXCONN);

	if (erStat != 0) {
		std::cerr << "Can't start to listen to. Error # " << WSAGetLastError() << std::endl;
		throw std::string("Can't start to listen to. Error # ");
	}

	while (true) {
		sockaddr_in clientInfo;
		ZeroMemory(&clientInfo, sizeof(clientInfo));

		int clientInfo_size = sizeof(clientInfo);

		SOCKET ClientConn = accept(Sock, (sockaddr*)&clientInfo, &clientInfo_size);

		if (ClientConn == INVALID_SOCKET) {
			std::cerr << "Client detected, but can't connect to a client. Error # " << WSAGetLastError() << std::endl;
			closesocket(ClientConn);
		}
		else {
			char clientIP[22];
			inet_ntop(AF_INET, &clientInfo.sin_addr, clientIP, INET_ADDRSTRLEN);

			try {
				auto client = std::make_shared<TalkToClient>(ClientConn, std::string(clientIP));
				new_client(client);
			}
			catch (std::string e) {
				if (e != "disconnected") {
					throw;
				}
			}
		}
	}
}

void Server::new_client(std::shared_ptr<TalkToClient> client)
{
	std::lock_guard lg(m);
	clients.insert({ client->get_name(), client });
	keys.push_back(client->get_name());

	view.update_computers(keys);
}

void Server::get_screenshot(int num)
{
	std::lock_guard lg(m);
	auto path = clients[keys[num]]->make_screenshot();

	view.update_screenshot_path(path);
}