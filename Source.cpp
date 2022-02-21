#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <iostream>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <string>


#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 100
#define DEFAULT_PORT "27015"

void help();
void myip(SOCKET dispSock);
void myport(SOCKET dispSock);
void cnt(std::string dest, int prtNumber);
void list();
void terminate(std::string id);
void send();



int main()
{

	// Initial variables
	// svr stands for variables related to Sever side of application
	// clnt stands for variables relate dto Client side of application
	WSADATA wsaData;

	int svrEC,
		clntEC;

	struct addrinfo* svrResult,
		* clntResult,
		* svrPtr,
		clntPtr,
		svrHint,
		clntHint;

        
	SOCKET svrListen = INVALID_SOCKET,
		clntListen = INVALID_SOCKET,
		svrConnect = INVALID_SOCKET,
		clntConnect = INVALID_SOCKET;


	// Initialize WSA
	svrEC = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (svrEC != 0)
	{
		printf("WSA start up failed: %d\n", svrEC);
		return 1;
	}
    

	// Placing information into server side addrinfo struct
    ZeroMemory(&svrHint, sizeof(svrHint));
	svrHint.ai_family = AF_INET;
	svrHint.ai_socktype = SOCK_STREAM;
	svrHint.ai_protocol = IPPROTO_TCP;
	svrHint.ai_flags = AI_PASSIVE;


	// Resolve the local address and port to be used by the server
	svrEC = getaddrinfo(NULL, DEFAULT_PORT, &svrHint, &svrResult);
	if (svrEC != 0)
	{
		printf("Getaddrinfo has failed: %d\n", svrEC);
		WSACleanup();
		return 1;
	}


	// Initializes the sever listen socket
	svrListen = socket(svrResult->ai_family, svrResult->ai_socktype, svrResult->ai_protocol);
	if (svrListen == INVALID_SOCKET)
	{
		printf("Sever Listen socket initialization failed: %d\n", WSAGetLastError());
		freeaddrinfo(svrResult);
		WSACleanup();
		return 1;
	}


	// Binds the listen socket
	svrEC = bind(svrListen, svrResult->ai_addr, svrResult->ai_addrlen);
	if (svrEC == SOCKET_ERROR)
	{
		printf("Binding of sever listen socket failed: %d\n", WSAGetLastError());
		freeaddrinfo(svrResult);
        closesocket(svrListen);
        WSACleanup();
		return 1;
	}


	// Frees the sever result pointer
	freeaddrinfo(svrResult);


	// Listens with the listen socket for connection on specific port
    svrEC = listen(svrListen, SOMAXCONN);
    if(svrEC == SOCKET_ERROR) {
        printf("Listen failed with error: %d\n", WSAGetLastError());
        closesocket(svrListen);
        WSACleanup();
        return 1;
        
    }

	//// Accepts conenction to client if successful listen
	//svrConnect == accept(svrListen, NULL, NULL);
	//if (svrConnect == INVALID_SOCKET) {
	//	printf("Sever failed to connect: %d\n", WSAGetLastError());
	//	closesocket(svrListen);
	//	WSACleanup();
	//	return 1;
	//}

    fd_set master;
    FD_ZERO(&master);

    FD_SET(listening, &master);

    while(true){//sever
        fd_set copy = master;

        int socketCount = select(0, &copy, nullptr, nullptr)
        for(int i = 0, i < socketCount, i++){
            svrConnect = copy.fd_array[i]
            if(svrConnect == svrListen){//accepts a connection
                SOCKET client = accepts(svrListen, nullptr, nullptr)

                FD_SET(client, &master);// copies client to master FD_SET

                string mssg = "Welcome";

                send(client, mssg.c_str(), mssg.size() + 1, 0)
            }else{// recieves a message
                char buf[4096];
                ZeroMemory(buf, 4096)

                int bytesIn = recv(svrConnect, buf, 4096, 0);
                if(bytesIn <= 0){//close connection if not receiving data
                    closesocket(svrConnect);
                    FD_CLR(svrConnect, &master);//clear the connection from the array


                }else{

                    for(int i = 0, i < )

                }
            }
        }

    }


	return 0;
}



void help() {
	
	return;
}

void myip(SOCKET dispSock) {

	return;
}

void myport(SOCKET dispSock) {

	return;
}

void cnt(std::string dest, int prtNumber) {

	return;
}

void list() {

	return;
}

void terminate(std::string id) {

	return;
}

void send() {

	return;
}