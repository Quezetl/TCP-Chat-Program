#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>


#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

using namespace std;

int main()
{
	string ipAddress;
	string clientPort;
	string serverPort;
	WSADATA wsaData;
	int ec,
		mode;

	struct addrinfo* sResult = NULL,
		* cResult = NULL ,
		* ptr = NULL,
		sHints,
		cHints;

	SOCKET ServerL = INVALID_SOCKET,
		Servercon = INVALID_SOCKET,
		Client = INVALID_SOCKET;


	// Starts up wsa to see if it works
	ec = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (ec != 0)
		cout << "WSAStartup failed: " << ec << endl;


	// Sets up hints for server & client
	ZeroMemory(&sHints, sizeof(sHints));
	sHints.ai_family = AF_INET;
	sHints.ai_socktype = SOCK_STREAM;
	sHints.ai_protocol = IPPROTO_TCP;
	sHints.ai_flags = AI_PASSIVE;

	ZeroMemory(&cHints, sizeof(cHints));
	cHints.ai_family = AF_UNSPEC;
	cHints.ai_socktype = SOCK_STREAM;
	cHints.ai_protocol = IPPROTO_TCP;


	// Ask the program what ports and ip address's to look on 
	cout << "Please input the ip address for looking for a connection: ";
	cin >> ipAddress;
	cout << "The address entered is : " << ipAddress << endl;
	cout << "Please input the port for client socket: ";
	cin >> clientPort;
	cout << "The port entered is : " << clientPort << endl;
	cout << "Please input the port for server socket: ";
	cin >> serverPort;
	cout << "The port entered is : " << serverPort << endl;
	

	// Resolve the local address and port to be used by the server
	ec = getaddrinfo(NULL, serverPort.c_str(), &sHints, &sResult);
	if (ec != 0) {
		cout << "Server getaddrinfo failed: " << ec << endl;
		WSACleanup();
		return 1;
	}
	ec = getaddrinfo(ipAddress.c_str(), clientPort.c_str(), &cHints, &cResult);
	if (ec != 0) {
		cout << "client side getaddrinfo failed: " << ec << endl;
		WSACleanup();
		return 1;
	}


	// Not too sure but Cresults needs to equate to ptr
	ptr = cResult;


	// Initialize sockets
	ServerL = socket(sResult->ai_family, sResult->ai_socktype, sResult->ai_protocol);
	if (ServerL == INVALID_SOCKET) {
		cout << "Error at the server side socket: " << WSAGetLastError() << endl;
		freeaddrinfo(sResult);
		WSACleanup();
		return 1;
	}
	Client = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
	if (Client ==SOCKET_ERROR) {
		cout << "Error at the client side socket: " << WSAGetLastError() << endl;
		freeaddrinfo(cResult);
		WSACleanup();
		return 1;
	}


	// Binds the tcp socket to addrinfo structs
	ec = bind(ServerL, sResult->ai_addr, sResult->ai_addrlen);
	if (ec == SOCKET_ERROR) {
		cout << "Binding for the server has failed: " << WSAGetLastError() << endl;
		freeaddrinfo(sResult);
		closesocket(ServerL);
		WSACleanup();
		return 1;
	}


	// Free up the address info results pointers
	freeaddrinfo(sResult);


	system("pause");
	// Output the follow prompts now
	cout << "Hello would you like to connect to a server or listen to a client?\n"
		<< "1) Listen for client.\n"
		<< "2) Connect to server.\n";
	cin >> mode;
	switch (mode) {
	case 1:
		cout << "You have chosen to listen for a client.\nListening for connection . . .";
		if (listen(ServerL, SOMAXCONN) == SOCKET_ERROR) {
			cout << "Server listen has failed: " << WSAGetLastError() << endl;
			closesocket(ServerL);
			WSACleanup();
			return 1;
		}
		Servercon = accept(ServerL, NULL, NULL);
		if (Servercon == INVALID_SOCKET) {
			cout << "Accepting of the socket has failed: " << WSAGetLastError() << endl;
			closesocket(ServerL);
			WSACleanup();
			return 1;
		}
		closesocket(ServerL);
		cout << "Socket has made connection\n";
		break;
	case 2:
		cout << "You have chosen to connect to a server.\n";
		ec = connect(Client, ptr->ai_addr, int(ptr->ai_addrlen));
		if (ec == SOCKET_ERROR) {
			closesocket(Client);
			freeaddrinfo(cResult);
			cout << "Connection to the server has failed!";
			WSACleanup();
			return 1;
		}
		break;
	default:
		cout << "Invalid option\n";
		break;
	}




	return 0;
}
