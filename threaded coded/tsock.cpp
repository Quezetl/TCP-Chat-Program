#include "tsock.h"

tsock::tsock(string port)
{
	// Initiate variables
	Serverport = port;
	sRes = NULL;
	cRes = NULL;
	cPtr = NULL;
	Listen = INVALID_SOCKET;
	WSADATA wsaData;
	search0 = "IPv4 Address. . . . . . . . . . . :";

	EC = WSAStartup(MAKEWORD(2, 2), &wsaData);
	
	// Error check
	if (EC != 0)
		cout << "wsaStartup has failed:" << endl;

	

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
				IPADD = line;
				IPFile.close();
			}
		}
	}
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

	// Error check
	if (EC != 0)
	{
		cout << "Getaddrinfo has failed:" << EC << endl;
		WSACleanup();
		return 1;
	}

	// Sets up listening socket for server
	Listen = socket(sRes->ai_family, sRes->ai_socktype, sRes->ai_protocol);

	// Error check
	if (Listen == INVALID_SOCKET)
	{
		cout << "Error at setting a listen socket for the server:" << WSAGetLastError() << endl;
		freeaddrinfo(sRes);
		WSACleanup();
		return 1;
	}

	// Sets up Listen socket for server
	EC = bind(Listen, sRes->ai_addr, sRes->ai_addrlen);

	// Error check
	if (EC == SOCKET_ERROR)
	{
		cout << "Binding for Server has failed:" << WSAGetLastError() << endl;
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
	// Error check
	if (listen(Listen, SOMAXCONN) == SOCKET_ERROR)
	{
		cout << "Listen failed with error:" << WSAGetLastError() << endl;
		return 1;
	}

	return 0;

	return 0;
}

int tsock::serverAccept()
{
	// Adds the newly connected client into the Connection vector
	Connection.push_back(accept(Listen, NULL, NULL));

	// Error check
	if (Connection.back() == INVALID_SOCKET && Connection.size() > 1)
	{
		cout << "Failed to connect: " << WSAGetLastError() << endl;
		return 1;
	}
	else if(Connection.back() != INVALID_SOCKET)
	{
		// Notify client that someone has connected
		cout << "Someone has connected to you!" << endl; 
		display();
	}
	return 0;
}

int tsock::display()
{
	do
	{
		// Receives the message sent from the connected client
		EC = recv(Connection.back(), servermsg, 100, 0);
		msg = servermsg;
		cIP = msg.substr(0, msg.find(' '));
		msg.erase(0, cIP.size()+1);
		cPort = msg.substr(0, msg.find(' '));
		msg.erase(0, cPort.size() + 1);


		if (EC > 0)
		{
			// Confirmation message sent to the client
			cout << "Message received from: " << cIP << endl
				<< "Sender's Port: "<< cPort <<endl
				<< "Message: " << msg << endl;

			// Error check
			sendEC = send(Connection.back(), "Message Received:", EC, 0);
			if (sendEC == SOCKET_ERROR)
			{
				cout << "Confirmation failed to send: " << WSAGetLastError() << endl;
				closesocket(Connection.back());
				return 1;
			}
		}
		else if (EC == 0)
			cout << "Connection closing:\n";
	} while (EC > 0);
	return 0;
}


// Prints the Help command
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
	cout << "IP: " << IPADD;
}

// Prints the client's port
void tsock::Myport()
{
	cout << "Current listening port: " << Serverport << endl << endl;
}


// Makes a connection between clinets
int tsock::Connect(string dest, string prt)
{
	// Sets up info for client side of things
	ZeroMemory(&cHint, sizeof(cHint));
	cHint.ai_family = AF_UNSPEC;
	cHint.ai_socktype = SOCK_STREAM;
	cHint.ai_protocol = IPPROTO_TCP;


	// Resolve the local address and the port to be used by the server
	EC = getaddrinfo(dest.c_str(), prt.c_str(), &cHint, &cRes);

	// Error check
	if (EC != 0)
	{
		cout << "Client side Getaddrinfo has failed:" << EC << endl;
		return 1;
	}

	// Redirects the pointer
	cPtr = cRes;

	// Adds new connected client into the ClientSock vector
	ClientSock.push_back(socket(cPtr->ai_family, cPtr->ai_socktype, cPtr->ai_protocol));

	// Error check
	if (ClientSock.back() == SOCKET_ERROR)
	{
		cout << "Error at the client side socket: " << WSAGetLastError() << endl;
		freeaddrinfo(cRes);
		return 1;
	}

	// Error check
	EC = connect(ClientSock.back(), cPtr->ai_addr, int(cPtr->ai_addrlen));
	if (EC == SOCKET_ERROR)
	{
		cout << "Connection to the server has failed";
		closesocket(ClientSock.back());
		return 1;
	}

	freeaddrinfo(cRes);

	//Error check
	if (ClientSock.back() == INVALID_SOCKET)
	{
		cout << "Unable to connect to server!\n";
		return 1;
	}

	cout << "Connection was successful!\n";

	// Temperory vector holding IP Addresses and port numbers of a client
	vector<string> temp;
	temp.push_back(dest);
	temp.push_back(prt);

	// A vector holding all clients Ip Address and port numbers
	ClientsInfo.push_back(temp);

	return 0;
}

// Displays a list of currently connected clients
void tsock::List()
{
	cout << "Id:" << setw(13) << "IP Adress"
		<< setw(13) << "Port No." << endl;

	for (int i = 0; i < ClientsInfo.size(); i++) {
		cout << " " << i + 1 << ":" << setw(13);

		// Prints IP addresses and port numbers
		for (int j = 0; j < ClientsInfo[i].size(); j++)
			cout << setw(13) << ClientsInfo[i][j];

	}
	cout << endl << endl;

}

//Terminates a selected connection
int tsock::Terminate(int id)
{
	// User input check
	if (id > ClientSock.size() || id == 0)
	{
		cout << "Invalid choice\n";
		return 1;
	}

	// Error check
	EC = shutdown(ClientSock.at(id - 1), SD_SEND);
	if (EC == SOCKET_ERROR)
	{
		cout << "A clients shutdown process has failed: " << WSAGetLastError() << endl;
		closesocket(ClientSock.at(id - 1));
		return 1;
	}

	// Deletes the selectes connection from ClientSock and ClientsInfo vectors
	closesocket(ClientSock.at(id - 1));
	ClientSock.erase(ClientSock.begin() + id - 1);
	ClientsInfo.erase(ClientsInfo.begin() + id - 1);

}


// Sends a message to a selected client
int tsock::Send(int id, string message)
{
	string newmsg = IPADD +  ' ' + Serverport + ' ' + message;
	if (id > ClientSock.size() || id == 0)
	{
		cout << "sorry no such client exists\n";
		return 0;
	}

	// Sends message to client
	EC = send(ClientSock.at(id - 1), (const char*)newmsg.c_str(), 100, 0);

	// Error check
	if (EC == SOCKET_ERROR)
	{
		cout << "Send failed: " << WSAGetLastError();
		return 1;
	}

	cout << "Message sent to connection: " << id <<"\n\n";
	return 0;
}

// Exits the application
int tsock::Exit()
{
	for (int i = 0; i < ClientSock.size(); i++)
	{
		if (ClientSock.at(i) != INVALID_SOCKET)
		{

			// Error check
			EC = shutdown(ClientSock.at(i), SD_SEND);
			if (EC == SOCKET_ERROR)
			{
				cout << "One of the clients shutdown process has failed: " << WSAGetLastError() << endl;
				closesocket(ClientSock.at(i));
			}
		}
		closesocket(ClientSock.at(i));
	}

	for (int i = 0; i < Connection.size(); i++)
	{
		if (Connection.at(i) != INVALID_SOCKET)
		{
			// Error check
			EC = shutdown(Connection.at(i), SD_SEND);
			if (EC == SOCKET_ERROR)
			{
				cout << "One of the servers shutdown process has failed: " << WSAGetLastError() << endl;
				closesocket(Connection.at(i));
			}
		}
	}


	closesocket(Listen);
	WSACleanup();
	return 0;
}

