
#include <WS2tcpip.h>
#include <string>
#include <sstream>

#pragma comment (lib, "ws2_32.lib")

class TCPListener
{

public:

	TCPListener(const char* ipAddress, int port) :
		m_ipAddress(ipAddress), m_port(port) { }

	// Initialize the listener
	int init();

	// Run the listener
	int run();

	// Main loop
	int mainLoop();
protected:

	// Handler for client connections
	virtual void onClientConnected(int clientSocket);

	// Handler for client disconnections
	virtual void onClientDisconnected(int clientSocket);

	// Handler for when a message is received from the client
	virtual void onMessageReceived(int clientSocket, const char* msg, int length, int CI);

	// Send a message to a client
	void sendToClient(int clientSocket, const char* msg, int length);

	// Broadcast a message from a client
	void findClient(int sendingClient, const char* msg, int length, int CI);

	void onHelp(int clientSocket);

private:

	const char*		m_ipAddress;	// IP Address server will run on
	int				m_port;			// Port # for the web service
	int				m_socket;		// Internal FD for the listening socket
	fd_set			m_master;		// Master file descriptor set
	std::string		option;
	std::string		response;
};
