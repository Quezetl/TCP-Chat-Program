//// Socket Practice.cpp : This file contains the 'main' function. Program execution begins and ends there.
////
//#ifndef WIN32_LEAN_AND_MEAN
//#define WIN32_LEAN_AND_MEAN
//#endif
//
//#include <iostream>
//#include <windows.h>
//#include <winsock2.h>
//#include <ws2tcpip.h>
//#include <iphlpapi.h>
//#include <stdio.h>
//
//#pragma comment(lib, "Ws2_32.lib")
//
//#define DEFAULT_BUFLEN 512
//#define DEFAULT_PORT "27015"
//
//using namespace std;
//
//
//int main()
//{
//	// Creates variable that will hold error codes if they occur
//	int iResult;
//
//	// Creates a WSADATA variable				WSA stands for windows server application
//	WSADATA wsaData;
//
//	//initiallizes the WSA
//	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
//
//	// Tests to make sure WSA was initialized correctly
//	if (iResult != 0)
//	{
//		cout << "WSA Start up failed: " << iResult << endl;
//		return 1;
//	}
//
//
//
//	// Creates addrinfo structure objects, used for server side shit
//	struct addrinfo* result = NULL, * ptr = NULL, hints;
//	//can also be formated as so:
//	//struct addrinfo* result = NULL;
//	//struct addrinfo* ptr = NULL;
//	//struct addrinfo hints;
//
//
//	// Initializes the hints struct
//	ZeroMemory(&hints, sizeof(hints));
//	hints.ai_family = AF_INET;
//	hints.ai_socktype = SOCK_STREAM;
//	hints.ai_protocol = IPPROTO_TCP;
//	hints.ai_flags = AI_PASSIVE;
//
//	// Resolve the local address and port to be used by the server
//	iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &result);						//default port was defined line 17 not a predetermined variable
//
//	// Tests to make sure get addr info was successful
//	if (iResult)
//	{
//		cout << "getaddrinfo failed: \n" << iResult << endl;
//		WSACleanup();
//		return 1;
//	}
//
//
//
//	// Creates a socket obj to listen for 1 connections
//	SOCKET ListenSocket = INVALID_SOCKET;
//
//	// Initializes the socket
//	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
//
//	// Tests the socket to make sure it was created successfully
//	if (ListenSocket == INVALID_SOCKET)
//	{
//		cout << "Error at socket(): " << WSAGetLastError() << endl;
//		freeaddrinfo(result);
//		WSACleanup();
//		return 1;
//	}
//
//
//
//	// Sets up the TCP listening socket
//	iResult = bind(ListenSocket, result->ai_addr, result->ai_addrlen);
//	if (iResult == SOCKET_ERROR)
//	{
//		cout << "bind failed with error: " << WSAGetLastError() << endl;
//		freeaddrinfo(result);
//		closesocket(ListenSocket);
//		WSACleanup();
//		return 1;
//	}
//
//	// Free up address info result ptr
//	freeaddrinfo(result);
//
//	// Listening on the socket
//	// this can be looped to be able to continuously check for connections
//	if (listen(ListenSocket, SOMAXCONN) == SOCKET_ERROR)
//	{
//		cout << "Listen has failed with error: " << WSAGetLastError() << endl;
//		closesocket(ListenSocket);
//		WSACleanup();
//		return 1;
//	}
//
//
//	// Creating a client socket that can be used to accept connections as opposed to just listening for one
//	SOCKET ClientSocket;
//	ClientSocket = INVALID_SOCKET;
//
//	// Testing to see if the client socket is accepted
//	ClientSocket = accept(ListenSocket, NULL, NULL);
//	if (ClientSocket == INVALID_SOCKET)
//	{
//		cout << "Accept failed: " << WSAGetLastError() << endl;
//		closesocket(ListenSocket);
//		WSACleanup();
//		return 1;
//	}
//
//	// Listening socket no longer required since connection has been accepted
//	closesocket(ListenSocket);
//
//
//
//	// Now its onto receiving and sending data on this socket to the client
//	//these are the variables used to store msg, read error msg, and set length of buffer
//	char recvbuf[DEFAULT_BUFLEN];					//default buffer length was defined by me at line 16, not built in variable
//	int iSendResult;
//	int recvbuflen = DEFAULT_BUFLEN;
//
//	// Receive until the peer shuts down the connection
//	do {
//
//		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
//		if (iResult > 0)
//		{
//			cout << "Bytes received: " << iResult << endl;
//			for (int i = 0; i < 14; i++)
//			{
//				cout << recvbuf[i];
//			}
//			cout << endl;
//
//
//			// Echo the buffer back to the sender
//			iSendResult = send(ClientSocket, recvbuf, iResult, 0);
//			if (iSendResult == SOCKET_ERROR)
//			{
//				cout << "Send failed: " << WSAGetLastError() << endl;
//				closesocket(ClientSocket);
//				WSACleanup();
//				return 1;
//			}
//			cout << "Bytes sent: " << iSendResult << endl;
//		}
//		else if (iResult == 0)
//			cout << "Connection closing ...\n";
//		else
//		{
//			cout << "Recv failed: " << WSAGetLastError << endl;
//			closesocket(ClientSocket);
//			WSACleanup();
//			return 1;
//		}
//
//	} while (iResult > 0);
//
//
//
//	// Time to disconnect the server
//	// Shutdown the send half of the connection since there is no more data to be sent
//	iResult = shutdown(ClientSocket, SD_SEND);
//	if (iResult == SOCKET_ERROR)
//	{
//		cout << "Shutdown failed: " << WSAGetLastError() << endl;
//		closesocket(ClientSocket);
//		WSACleanup();
//		return 1;
//	}
//	// Note the server can still receive information, but now it can no longer send
//
//
//
//	system("pause");
//	// Cleanup
//	// Now we close the receiving portion of the server
//	closesocket(ClientSocket);
//	WSACleanup();
//	return 0;
//}
//
//
//// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
//// Debug program: F5 or Debug > Start Debugging menu
//
//// Tips for Getting Started: 
////   1. Use the Solution Explorer window to add/manage files
////   2. Use the Team Explorer window to connect to source control
////   3. Use the Output window to see build output and other messages
////   4. Use the Error List window to view errors
////   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
////   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
