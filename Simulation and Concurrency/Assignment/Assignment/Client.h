#pragma once

#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <winsock2.h>
#include <fstream>

class Client
{
private:
	std::ofstream f;

public:
	Client();
	~Client();

	int Initialize();
	void Send();
	void Clean();

	sockaddr_in peer;
};

