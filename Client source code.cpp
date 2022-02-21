//// TCP Socket - Client.cpp : This file contains the 'main' function. Program execution begins and ends there.
////
//#ifndef WIN32_LEAN_AND_MEAN
//#define WIN32_LEAN_AND_MEAN
//#endif
//
//#include <windows.h>
//#include <winsock2.h>
//#include <ws2tcpip.h>
//#include <stdlib.h>
//#include <stdio.h>
//#include <iostream>
//
//
//#pragma comment(lib, "Ws2_32.lib")
//
//#define DEFAULT_BUFLEN 512
//#define DEFAULT_PORT "27015"
//
//
//using namespace std;
//
//int main(int argc, char* argv[])
//{
//	// Creates WSAData obj and error code obj
//	WSADATA wsaData;
//	int iResult;
//
//	// Initializes winsock
//	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
//	if (iResult != 0)
//	{
//		cout << "WSAStartup has failed: " << iResult << endl;
//		return 1;
//	}
//
//
//	// Creates the addrinfo structs to create socket
//	struct addrinfo* result = NULL,
//		* ptr = NULL,
//		hints;
//
//	// Initializes and sets up addrinfo obj
//	ZeroMemory(&hints, sizeof(hints));
//	hints.ai_family = AF_UNSPEC;
//	hints.ai_socktype = SOCK_STREAM;
//	hints.ai_protocol = IPPROTO_TCP;
//
//	// Resolves the server address and port
//	iResult = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &result);
//
//	// Checks for errors with getaddrinfo intialization
//	if (iResult != 0)
//	{
//		cout << "GetAddrInfo has failed: " << iResult << endl;
//		WSACleanup();
//		return 1;
//	}
//
//
//	// Creation of connection socket
//	SOCKET ConnectionSocket = INVALID_SOCKET;
//
//	// Attempt to connect to the first address returned by
//	// the call to getaddrinfo
//	ptr = result;
//
//	// Create a SOCKET for connecting to server
//	ConnectionSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
//
//	// Check for errors
//	if (ConnectionSocket == SOCKET_ERROR)
//	{
//		cout << "Error at the socket: " << WSAGetLastError() << endl;
//		freeaddrinfo(result);
//		WSACleanup();
//		return 1;
//	}
//
//
//
//	// Connect to server
//	iResult = connect(ConnectionSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
//	if (iResult == SOCKET_ERROR)
//	{
//		closesocket(ConnectionSocket);
//		ConnectionSocket = INVALID_SOCKET;
//	}
//
//	// Should really try the next address returned by getaddrinfo
//	// if the connect call failed
//	// But for this simple example we just free the resources
//	// returned by getaddrinfo and print an error msg
//
//	freeaddrinfo(result);
//
//	if (ConnectionSocket == INVALID_SOCKET)
//	{
//		cout << "Unable to connect to server";
//		WSACleanup();
//		return 1;
//	}
//
//
//	// Create receiving buffer length
//	int recvbuflen = DEFAULT_BUFLEN;
//
//	// String to be sent & creation of receiving buf
//	const char* sendbuf = "This is a test";
//	char recvbuf[DEFAULT_BUFLEN];
//
//	// Error testing send command
//	iResult = send(ConnectionSocket, sendbuf, (int)strlen(sendbuf), 0);
//	if (iResult == SOCKET_ERROR)
//	{
//		cout << "Send failed: " << WSAGetLastError() << endl;
//		closesocket(ConnectionSocket);
//		WSACleanup();
//		return 1;
//	}
//
//	// Outputs a successful result
//	cout << "Bytes sent: " << iResult << endl;
//
//	// Shutdown the connection for sending since no more data will be sent
//	// the client can still use the ConnectionSocket for receiving data
//	iResult = shutdown(ConnectionSocket, SD_SEND);
//	if (iResult == SOCKET_ERROR)
//	{
//		cout << "Shutdown failed: " << WSAGetLastError() << endl;
//		closesocket(ConnectionSocket);
//		WSACleanup();
//		return 1;
//	}
//
//	// Receive data until the server closes the connection
//	do {
//		iResult = recv(ConnectionSocket, recvbuf, recvbuflen, 0);
//		if (iResult > 0)
//			cout << "Bytes received: " << iResult << endl;
//		else if (iResult == 0)
//			cout << "Connection closed\n";
//		else
//			cout << "recv failed: " << WSAGetLastError() << endl;
//	} while (iResult > 0);
//
//	system("pause");
//
//	// Cleanup
//	closesocket(ConnectionSocket);
//	WSACleanup();
//	return 0;
//}
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