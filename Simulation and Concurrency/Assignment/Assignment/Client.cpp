#include "Client.h"

using namespace std;

Client::Client()
{
	f.open("ClientLog.txt");
}


Client::~Client()
{
	f.close();
}

int Client::Initialize()
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

	// Create socket data space		// Socket a cui mi connetto
	
	peer.sin_family = AF_INET;
	peer.sin_port = htons(9171);	// port 9171
	peer.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	f << "Connected! \n";
}

void Client::Send()
{
	while (1)
	{
		// Create transfer socket
		char buffer;
		char derpy[10] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i' };
		int i = rand() % 10;
		SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
		if (s == INVALID_SOCKET) {
			cerr << "Create socket failed" << endl;
			f << "Create socket failed \n";
		}
		else if (connect(s, (sockaddr *)&peer, sizeof(peer)) == SOCKET_ERROR) {
			cerr << "Connect to peer failed with " << WSAGetLastError() << endl;
			f << "Connect to peer failed with " << WSAGetLastError() << "\n";
		}
		else if (send(s, &derpy[i], 1, 0) == SOCKET_ERROR) {
			cerr << "Send failed with " << WSAGetLastError() << endl;
			f << "Send failed with " << WSAGetLastError() << "\n";
		}
		else if (recv(s, &buffer, 1, 0) == SOCKET_ERROR) {
			cerr << "Receive failed with " << WSAGetLastError() << endl;
			f << "Receive failed with " << WSAGetLastError();
		}
		else {
			cout << "Message= " << buffer << endl;
			// writing in a file what is going on
			f << "Message= " << buffer << "\n";
		}
	}
}