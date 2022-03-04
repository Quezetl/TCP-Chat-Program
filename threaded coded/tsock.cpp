tsock::tsock(string port)
{
	// Initiate variables
	Serverport = port;
	sRes = NULL;
	cRes = NULL;
	cPtr = NULL;
	Listen = INVALID_SOCKET;
	WSADATA wsaData;

	EC = WSAStartup(MAKEWORD(2, 2), &wsaData);
	
	// Error check
	if (EC != 0)
		cout << "wsaStartup has failed:" << endl;
}

//initiates server side
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

	//char ipstringbuffer[46];
	//DWORD ipbufferlength = 46;
	//DWORD dwRetval;

	//struct sockaddr_in* sockaddr_ipv4;
	//struct addrinfo* result = NULL;
	//struct addrinfo* ptr = NULL;
	//struct addrinfo hints;

	//dwRetval = getaddrinfo(NULL, Serverport.c_str(), &hints, &result);
	//sockaddr_ipv4 = (struct sockaddr_in*)ptr->ai_addr;
	//printf("\tIPv4 address %s\n", inet_ntoa(sockaddr_ipv4->sin_addr)); 


	freeaddrinfo(sRes);
	return 0;
}

// Listens for a client connection
int tsock::serverListen()
{
	// Error check
	if (listen(Listen, SOMAXCONN) == SOCKET_ERROR)
	{
		cout << "Listen failed with error:" << WSAGetLastError() << endl;
		closesocket(Listen);
		WSACleanup();
		return 1;
	}

	return 0;
}

// Accepts a Connection and adds it to a vector of connections
int tsock::serverAccept()
{
	// Adds the newly connected client into the Connection vector
	Connection.push_back(accept(Listen, NULL, NULL));

	// Error check
	if (Connection.back() == INVALID_SOCKET)
	{
		cout << "Failed to accept connection:" << WSAGetLastError() << endl;
		closesocket(Listen);
		WSACleanup();
		return 1;
	}
	
	display();
	return 0;
}

// General message sending and receiving
int tsock::display()
{
	do
	{
		// Receives the message sent from the connected client
		EC = recv(Connection.back(), servermsg, 100, 0);
		if (EC > 0)
		{
			// Confirmation message sent to the client
			cout << setw(10) << "Message:" << servermsg << endl;

			// Error check
			sendEC = send(Connection.back(), "Message Received:", EC, 0);
			if (sendEC == SOCKET_ERROR)
			{
				cout << "Confirmation failed to send:\n" << WSAGetLastError();
				closesocket(Connection.back());
				WSACleanup();
				return 1;
			}
		}
		else if (EC == 0)
			cout << "Connection closing:\n";
		else
		{
			cout << "Failed to receive message:\n" << WSAGetLastError();
			closesocket(Connection.back());
			WSACleanup();
			return 1;
		}
	} while (EC > 0);
	return 0;
}

// Prints the Help command
void tsock::Help()
{
	cout << "Options:\n"
		<< "Help: Brings up list of all commands available.\n"
		<< "Myip: Displays IP address of current process.\n"
		<< "Myport: Displays the port that this process is listening to for connections.\n"
		<< "Connect <destination> <port number>: Establishes new connection to specified destination at specified port.\n"
		<< "List: Lists all currently connected connection id's and associated port numbers.\n"
		<< "Terminate <connection id>: Terminates specified connection.\n"
		<< "Send <connection id> <message>: Sends a message (up to 100 bytes) to specified host.\n"
		<< "Exit: Closes all connections and ends the process.\n\n";
}

//IN Progress
void tsock::Myip()
{



}

// Prints the client's port
void tsock::Myport()
{
	cout << "Current listening port: " << setw(10) << Serverport << endl << endl;
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
		WSACleanup();
		return 1;
	}

	// Error check
	EC = connect(ClientSock.back(), cPtr->ai_addr, int(cPtr->ai_addrlen));
	if (EC == SOCKET_ERROR)
	{
		cout << "Connection to the server has failed";
		closesocket(ClientSock.back());
		freeaddrinfo(cRes);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(cRes);

	//Error check
	if (ClientSock.back() == INVALID_SOCKET)
	{
		cout << "Unable to connect to server!\n";
		WSACleanup();
		return 1;
	}

	cout << "Connection was successful!\n";
	tsock::Send(ClientSock.size(), "A new Connection has been made!\n");

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
		cout << setw(13) << i + 1;

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
		cout << "A clients shutdown process has failed: " << WSAGetLastError();
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
	if (id > ClientSock.size() || id == 0)
	{
		cout << "sorry no such client exists\n";
		return 0;
	}

	// Send the message that you wish
	EC = send(ClientSock.at(id - 1), (const char*)message.c_str(), 100, 0);

	// Error check
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
			// Error check
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
