#include "Chat.h"
#include <string>

void Chat::onClientConnected(int clientSocket)// when client first connest to the sever
{
	// Send a welcome message to the connected client
	std::string welcomeMsg = "Welcome to the Awesome Chat Server!\r\n";

	sendToClient(clientSocket, welcomeMsg.c_str(), welcomeMsg.size() + 1);
}

void Chat::onClientDisconnected(int clientSocket)
{

}

void Chat::onMessageReceived(int clientSocket, const char* msg, int length, int CI)
{
	findClient(clientSocket, msg, length, CI);
}
