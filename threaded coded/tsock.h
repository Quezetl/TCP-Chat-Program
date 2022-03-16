#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <WinSock2.h>
#include <string>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <ws2tcpip.h>
#include <vector>
#include <iomanip>
#include <stdio.h>

#pragma comment(lib, "Ws2_32.lib")
using namespace std;

class tsock
{
public:
	// Initiations
	tsock(string port);
	int Server_init();
	int serverListen();
	int serverAccept();
	int display();

	// Client prompts
	void Help();
	void Myip();
	void Myport();
	int Connect(string dest, string prt);
	void List();
	int Terminate(int id);
	int Send(int id, string message);
	int Exit();

protected:

private:
	int							EC;
	int							sendEC;
	int							offset;
	char						servermsg[100];
	string						Clientport;
	string						Serverport;
	string						IPADD;
	string						line;
	string						search0;
	string						cIP;
	string						cPort;
	string						msg;
	ifstream					IPFile;
	addrinfo* sRes,
		* cRes,
		* cPtr,
		* sPtr,
		sHint,
		cHint;
	SOCKET						Listen;
	vector<SOCKET>				Connection;
	vector<SOCKET>				ClientSock;
	vector<vector<string>>		ClientsInfo;
};
