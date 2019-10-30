#include "Server.h"

using namespace std;

Server::Server()
{
	f.open("ServerLog.txt");
}


Server::~Server()
{
	f.close();
}

int Server::Initialize()
{
	// Create version identifier
	WORD wVersionRequested = MAKEWORD(2, 0);

	// Startup windows sockets
	WSADATA wsaData;
	if (WSAStartup(wVersionRequested, &wsaData)) {
		cerr << "Socket initialisation failed" << endl;
		f << "Socket initialisation failed \n";
		return -1;
	}
	peer.sin_family = AF_INET;
	peer.sin_port = htons(9171);	// port 9171
	peer.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	f << "Socket created \n";
}

void Server::ListenEcho()
{
	CreateListeningSocket();
}

void Server::CreateListeningSocket()
{
	// Create listening socket
	s = socket(AF_INET, SOCK_STREAM, 0);
	if (s == INVALID_SOCKET) {
		cerr << "Create socket failed" << endl;
		f << "Create socket failed \n";
	}
	else if (bind(s, (sockaddr *)&peer, sizeof(peer)) == SOCKET_ERROR) {
		cerr << "Bind failed with " << WSAGetLastError() << endl;
		f << "Bind failed with " << WSAGetLastError() << "\n";
	}
	else if (listen(s, 5) == SOCKET_ERROR) {
		cerr << "Listen failed with " << WSAGetLastError() << endl;
		f << "Listen failed with " << WSAGetLastError() << "\n";
	}
	else
		CreateTransferSocket();
}

void Server::CreateTransferSocket()
{
	while (1)
	{
		s1 = accept(s, NULL, NULL);	// 
		if (s1 == INVALID_SOCKET) {
			cerr << "Accept failed with " << WSAGetLastError() << endl;
			f << "Accept failed with " << WSAGetLastError() << "\n";
		}
		else if (recv(s1, &buffer, 1, 0) == SOCKET_ERROR) {
			cerr << "Receive failed with " << WSAGetLastError() << endl;
			f << "Receive failed with " << WSAGetLastError() << "\n";
		}
		else {
			cout << "Message= " << buffer << endl;
			// writing in a file what is going on
			f << "Message= " << buffer << "\n";
			if (send(s1, &buffer, 1, 0) == SOCKET_ERROR) {
				cerr << "Send failed with " << WSAGetLastError() << endl;
			}
		}
		// Delay
		cout << "Waiting..." << endl; char ch; cin >> ch;
		f << "Waiting... \n ";
	}
}

void Server::Clean()
{
	// Cleanup windows sockets
	WSACleanup();
}