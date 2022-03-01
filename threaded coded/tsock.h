#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <WinSock2.h>
#include <string>
#include <iostream>
#include <Windows.h>
#include <ws2tcpip.h>
#include <thread>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

class tsock
{
public:
	tsock(string port);
	int Server_init();
	int Client_init(string ip, string prt);
	int serverListen();
	int serverAccept();
	int display();
	void Help();
	void Myip();
	void Myport();
	int Connect(string dest, string prt);
	void List();
	void Terminate();
	int Send(int id, string message);
	int Exit();

protected:

private:
	int			EC;
	int			sendEC;
	char       *msg[100];
	char		servermsg[100];
	string		Ipadd;
	string		Clientport;
	string		Serverport;
	addrinfo   *sRes,
			   *cRes,
			   *cPtr,
				sHint,
				cHint;
				
	SOCKET		Listen,
				Search,
				Connection,
				Receive;

};

