#pragma once

#pragma comment(lib, "ws2_32.lib")
#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <winsock2.h>
#include <fstream>

class Server
{
private:
	std::ofstream f;
	sockaddr_in peer;
	SOCKET s, s1;
	char buffer;

public:
	Server();
	~Server();

	int Initialize();
	void Listen();
	void ListenEcho();
	void CreateListeningSocket();
	void CreateTransferSocket();
	void Clean();
};

