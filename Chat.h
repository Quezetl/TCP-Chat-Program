#pragma once
#include "TCPListener.h"

class Chat : public TCPListener
{
public:

	Chat(const char* ipAddress, int port) :
		TCPListener(ipAddress, port) { }

protected:

	// Handler for client connections
	virtual void onClientConnected(int clientSocket);

	// Handler for client disconnections
	virtual void onClientDisconnected(int clientSocket);

	// Handler for when a message is received from the client
	virtual void onMessageReceived(int clientSocket, const char* msg, int length, int CI);
};
