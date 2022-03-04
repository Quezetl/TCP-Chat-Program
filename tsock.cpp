#include "tsock.h"

tsock::tsock(string port)
{
	Serverport = port;
	sRes = NULL;
	cRes = NULL;
	cPtr = NULL;
	Listen = INVALID_SOCKET;
	WSADATA wsaData;

	EC = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (EC != 0)
		cout << "wsaStartup has failed\n";
}

int tsock::Server_init()
{
	// Sets up ip address info
	ZeroMemory(&sHint, sizeof(sHint));
	sHint.ai_family = AF_INET;
	sHint.ai_socktype = SOCK_STREAM;
	sHint.ai_protocol = IPPROTO_TCP;
	sHint.ai_flags = AI_PASSIVE;


	// Sets the server side ip info
	EC = getaddrinfo(NULL, Serverport.c_str(), &sHint, &sRes);
	if (EC != 0)
	{
		cout << "Getaddrinfo has failed: " << EC;
		WSACleanup();
		return 1;
	}


	// Sets up listening socket for server
	Listen = socket(sRes->ai_family, sRes->ai_socktype, sRes->ai_protocol);
	if (Listen == INVALID_SOCKET)
	{
		cout << "Error at setting a listen socket for the server: " << WSAGetLastError() << endl;
		freeaddrinfo(sRes);
		WSACleanup();
		return 1;
	}


	// Sets up Listen socket for server
	EC = bind(Listen, sRes->ai_addr, sRes->ai_addrlen);
	if (EC == SOCKET_ERROR)
	{
		cout << "Binding on port: " << Serverport << "has failed\nError code: "<< WSAGetLastError() << endl;
		freeaddrinfo(sRes);
		closesocket(Listen);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(sRes);


	return 0;
}


int tsock::serverListen()
{
	if (listen(Listen, SOMAXCONN) == SOCKET_ERROR)
	{
		cout << "Listen failed with error: " << WSAGetLastError();
		closesocket(Listen);
		WSACleanup();
		return 1;
	}

	return 0;
}

int tsock::serverAccept()
{	
	addrinfo r;
	ZeroMemory(&r, sizeof(r)); 
	r.ai_family = AF_UNSPEC;
	r.ai_socktype = SOCK_STREAM;
	r.ai_protocol = IPPROTO_TCP;
	Connection.push_back(accept(Listen, r.ai_addr,(int*)r.ai_addrlen));
	cout << r.ai_addr;
	if (Connection.back() == INVALID_SOCKET)
	{
		cout << "accept failed: " << WSAGetLastError();
		return 1;
	}
	//
	display();
	return 0;
}

int tsock::display()
{
	do
	{
		// Receives the message sent from the connected client
		EC = recv(Connection.back(), servermsg, 100, 0);
		if (EC > 0)
		{
			// Confirmation message sent to the client
			cout << "Message: " << servermsg << endl;
			sendEC = send(Connection.back(), "Message Received", EC, 0);
			if (sendEC == SOCKET_ERROR)
			{
				cout << "Confirmation failed to send: " << WSAGetLastError();
				closesocket(Connection.back());
				return 1;
			}
		}
		else if (EC == 0)
			cout << "Connection closing from: \n" << Connection.back();
		else
		{
			cout << "Receiving failed: " << WSAGetLastError();
			closesocket(Connection.back());
			return 1;
		}
	} while (EC > 0);
	return 0;
}

void tsock::Help()
{
	cout << "Options:\n"
		<< "help: brings up list of all commands available.\n"
		<< "myip: Displays IP address of current process.\n"
		<< "myport: Displays the port that this process is listening to for connections.\n"
		<< "connect <destination> <port number>: Establishes new connection to specified destination at specified port.\n"
		<< "list: Lists all currently connected connection id's and associated port numbers.\n"
		<< "terminate <connection id>: Terminates specified connection.\n"
		<< "send <connection id> <message>: Sends a message (up to 100 bytes) to specified host.\n"
		<< "exit: Closes all connections and ends the process.\n\n";
}

void tsock::Myip()
{

	string line;
	ifstream IPFile;
	int offset;
	string search0 = "IPv4 Address. . . . . . . . . . . :";      // search pattern

	system("ipconfig > ip.txt");

	IPFile.open("ip.txt");
	if (IPFile.is_open())
	{
		while (!IPFile.eof())
		{
			getline(IPFile, line);
			if ((offset = line.find(search0.c_str(), 0)) != string::npos)
			{
				//   IPv4 Address. . . . . . . . . . . : 1
				//1234567890123456789012345678901234567890     
				line.erase(0, 39);
				cout << line << endl;
				IPFile.close();
			}
		}
	}
}

void tsock::Myport()
{
	cout << "Current listening port: " << Serverport << endl << endl;
}

int tsock::Connect(string dest, string prt)
{
	// Sets up info for client side of things
	ZeroMemory(&cHint, sizeof(cHint));
	cHint.ai_family = AF_UNSPEC;
	cHint.ai_socktype = SOCK_STREAM;
	cHint.ai_protocol = IPPROTO_TCP;


	// Resolve the local address and the port to be used by the server
	EC = getaddrinfo(dest.c_str(), prt.c_str(), &cHint, &cRes);
	if (EC != 0)
	{
		cout << "Client side Getaddrinfo has failed: " << EC << endl;
		return 1;
	}

	// Redirects the pointer
	cPtr = cRes;

	ClientSock.push_back(socket(cPtr->ai_family, cPtr->ai_socktype, cPtr->ai_protocol));
	if (ClientSock.back() == SOCKET_ERROR)
	{
		cout << "Error at the client side socket: " << WSAGetLastError() << endl;
		return 1;
	}
	EC = connect(ClientSock.back(), cPtr->ai_addr, int(cPtr->ai_addrlen));
	if (EC == SOCKET_ERROR)
	{
		cout << "Connection to the server has failed";
		closesocket(ClientSock.back());
		return 1;
	}
	
	freeaddrinfo(cRes);

	if (ClientSock.back() == INVALID_SOCKET)
	{
		cout << "unable to connect to server!\n";
		return 1;
	}

	cout << "Connection was successful!\n";
	tsock::Send(ClientSock.size(), "A new Connection has been made!\n");

	vector<string> temp;
	temp.push_back(dest);//ip address
	temp.push_back(prt);//port
	ClientsInfo.push_back(temp);

	return 0;
}

void tsock::List()
{
	cout.width(3);
	cout << "Id:";
	cout.width(17);
 	cout << "IP Address:";
	cout.width(12);
	cout << "Port No.\n";
	for(int i = 0; i < ClientsInfo.size(); i++)
	{
		cout.width(3);
		cout << right << i + 1;
		cout.width(17);
		cout << right << ClientsInfo[i][0];
		cout.width(11);
		cout << right << ClientsInfo[i][1];
		cout << endl;
	}
	cout << endl << endl;
}

int tsock::Terminate(int id)
{
	if (id > ClientSock.size() || id == 0)
	{
		cout << "Invalid choice\n";
		return 1;
	}
	EC = shutdown(ClientSock.at(id - 1), SD_SEND);
	if (EC == SOCKET_ERROR)
	{
		cout << "One of the clients shutdown process has failed: " << WSAGetLastError();
		closesocket(ClientSock.at(id - 1));
		return 1;
	}
	closesocket(ClientSock.at(id - 1));
	ClientSock.erase(ClientSock.begin() + id - 1);
	ClientsInfo.erase(ClientsInfo.begin() + id - 1);
	return 0;
}

int tsock::Send(int id, string message)
{
	if (id > ClientSock.size() || id == 0)
	{
		cout << "sorry no such client exists\n";
		return 0;
	}
	// Send the message that you wish
	EC = send(ClientSock.at(id-1), (const char*)message.c_str(), 100, 0);
	if (EC == SOCKET_ERROR)
	{
		cout << "Send failed: " << WSAGetLastError();
		return 1;
	}
	cout << "Message sent\n\n";
	return 0;
}

int tsock::Exit()
{
	for (int i = 0; i < ClientSock.size(); i++)
	{
		if (ClientSock.at(i) != INVALID_SOCKET)
		{
			EC = shutdown(ClientSock.at(i), SD_SEND);
			if (EC == SOCKET_ERROR)
			{
				cout << "One of the clients shutdown process has failed: " << WSAGetLastError();
				closesocket(ClientSock.at(i));
			}
		}
		closesocket(ClientSock.at(i));
	}
	
	for (int i = 0; i < Connection.size(); i++)
	{
		if (Connection.at(i) != INVALID_SOCKET)
		{
			EC = shutdown(Connection.at(i), SD_SEND);
			if (EC == SOCKET_ERROR)
			{
				cout << "One of the servers shutdown process has failed: " << WSAGetLastError();
				closesocket(Connection.at(i));
			}
		}
	}


	closesocket(Listen);
	WSACleanup();
	return 0;
}
