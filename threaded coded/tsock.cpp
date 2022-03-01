#include "tsock.h"

tsock::tsock(string port)
{
	Serverport = port;
	Ipadd = "127.0.0.1";
	sRes = NULL;
	cRes = NULL;
	cPtr = NULL;
	Listen = INVALID_SOCKET;
	Search = INVALID_SOCKET;
	Connection = INVALID_SOCKET;
	Receive = INVALID_SOCKET;
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
		cout << "Binding for Server has failed: " << WSAGetLastError() << endl;
		freeaddrinfo(sRes);
		closesocket(Listen);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(sRes);

	return 0;
}



int tsock::Client_init(string ip, string prt)
{
	// Sets up info for client side of things
	ZeroMemory(&cHint, sizeof(cHint));
	cHint.ai_family = AF_UNSPEC;
	cHint.ai_socktype = SOCK_STREAM;
	cHint.ai_protocol = IPPROTO_TCP;


	// Resolve the local address and the port to be used by the server
	EC = getaddrinfo(ip.c_str(), prt.c_str(), &cHint, &cRes);
	if (EC != 0)
	{
		cout << "Client side Getaddrinfo has failed: " << EC << endl;
		return 1;
	}

	// Redirects the pointer
	cPtr = cRes;

	Search = socket(cPtr->ai_family, cPtr->ai_socktype, cPtr->ai_protocol);
	if (Search == SOCKET_ERROR)
	{
		cout << "Error at the client side socket: " << WSAGetLastError() << endl;
		freeaddrinfo(cRes);
		WSACleanup();
		return 1;
	}
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
	serverAccept();
	return 0;
}

int tsock::serverAccept()
{
	Connection = accept(Listen, NULL, NULL);
	if (Connection == INVALID_SOCKET)
	{
		cout << "accept failed: " << WSAGetLastError();
		closesocket(Listen);
		WSACleanup();
		return 1;
	}
	display();
	return 0;
}

int tsock::display()
{
	do
	{
		EC = recv(Connection, servermsg, 100, 0);
		if (EC > 0)
		{
			cout << "Message: " << servermsg << endl;
			sendEC = send(Connection, "Message Received", EC, 0);
			if (sendEC == SOCKET_ERROR)
			{
				cout << "Confirmation failed to send: " << WSAGetLastError();
				closesocket(Connection);
				WSACleanup();
				return 1;
			}
		}
		else if (EC == 0)
			cout << "Connection closing\n";
		else
		{
			cout << "Receiving failed: " << WSAGetLastError();
			closesocket(Connection);
			WSACleanup();
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

	Search = socket(cPtr->ai_family, cPtr->ai_socktype, cPtr->ai_protocol);
	if (Search == SOCKET_ERROR)
	{
		cout << "Error at the client side socket: " << WSAGetLastError() << endl;
		freeaddrinfo(cRes);
		WSACleanup();
		return 1;
	}
	EC = connect(Search, cPtr->ai_addr, int(cPtr->ai_addrlen));
	if (EC == SOCKET_ERROR)
	{
		cout << "Connection to the server has failed";
		closesocket(Search);
		freeaddrinfo(cRes);
		WSACleanup();
		return 1;
	}
	
	freeaddrinfo(cRes);

	if (Search == INVALID_SOCKET)
	{
		cout << "unable to connect to server!\n";
		WSACleanup();
		return 1;
	}
	return 0;
}

void tsock::List()
{

}

void tsock::Terminate()
{

}

int tsock::Send(int id, string message)
{
	// Send the message that you wish
	EC = send(Search, (const char*)message.c_str(), 100, 0);
	if (EC == SOCKET_ERROR)
	{
		cout << "Send failed: " << WSAGetLastError();
		closesocket(Listen);
		WSACleanup();
		return 1;
	}
	cout << "Message sent\n\n";
	return 0;
}

int tsock::Exit()
{

	if (!(Search == INVALID_SOCKET))
	{
		EC = shutdown(Search, SD_SEND);
		if (EC == SOCKET_ERROR)
		{
			cout << "Client shutdown failed: " << WSAGetLastError();
			closesocket(Search);
			WSACleanup();
			return 1;
		}
	}
	

	if (!(Listen == INVALID_SOCKET))
	{
		EC = shutdown(Listen, SD_SEND);
		if (EC == SOCKET_ERROR)
		{
			cout << "Server shutdown failed: " << WSAGetLastError();
			closesocket(Listen);
			WSACleanup();
			return 1;
	
		}
	}

	closesocket(Listen);
	closesocket(Search);
	WSACleanup();
	return 0;
}
