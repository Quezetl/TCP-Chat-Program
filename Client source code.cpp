#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <string>
#include <vector>


// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")


#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"

WSADATA wsaData;
SOCKET ConnectSocket = INVALID_SOCKET;
struct addrinfo* result = NULL,
    * ptr = NULL,
    hints;
const char* sendbuf = "this is a test";
char recvbuf[DEFAULT_BUFLEN];
int iResult;
int recvbuflen = DEFAULT_BUFLEN;
std::vector<std::string> list_IP;
std::vector<int> list_PORT;


int __cdecl main(int argc, char** argv)
{

    //User input 
    //



    // Validate the parameters
    if (argc != 2) {
        printf("usage: %s server-name\n", argv[0]);

        //return 1;
    }

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed with error: %d\n", iResult);
        // return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        //return 1;
    }

    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            //return 1;
        }

        // Connect to server.
        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        //return 1;
    }

    // Send an initial buffer
    iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        //return 1;
    }

    printf("Bytes Sent: %ld\n", iResult);

    // shutdown the connection since no more data will be sent
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        //return 1;
    }

    // Receive until the peer closes the connection
    do {

        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0)
            printf("Bytes received: %d\n", iResult);
        else if (iResult == 0)
            printf("Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());

    } while (iResult > 0);
    system("pause");
    // cleanup

    //return 0;
}

void help() {
    std::cout << "Options:\n"
        << "help: brings up list of all commands available.\n"
        << "myip: Displays IP address of current process.\n"
        << "myport: Displays the port that this process is listening to for connections.\n"
        << "connect <destination> <port number>: Establishes new connection to specified destination at specified port.\n"
        << "list: Lists all currently connected connection id's and associated port numbers.\n"
        << "terminate <connection id>: Terminates specified connection.\n"
        << "send <connection id> <message>: Sends a message (up to 100 bytes) to specified host.\n"
        << "exit: Closes all connections and ends the process.\n";
    return;
}

void myip(SOCKET dispSock) {
    // no clue 

    return;
}

void myport() {
    std::cout << DEFAULT_PORT;
    return;
}

void cnt(std::string dest, int prtNumber, std::vector<std::string> list_IP, std::vector<int> list_PORT, SOCKET connectsocket) {
    list_IP.push_back(dest);
    list_PORT.push_back(prtNumber);
    /*


            here goes the function for setting up a connection




    */
    return;
}

// DONE
void list() {
    std::cout << "id: IP address\t\t\t" << std::right << "Port No.\n";
    for (int i = 0; i < list_IP.size(); i++)
    {
        std::cout << i + 1 << ": " << list_IP[i] << "\t\t\t" << std::right << list_PORT[i] << std::endl;
    }
    return;
}

void terminate(int id) {
    if (id > list_IP.size() - 1)
    {
        std::cout << "This connection does not exist try another";
        return;
    }

    /*

        using the id int find that socket and terminate it ie: closesocket(socket[id])

    */

    // After successful termination we will get rid of that id in the vectors
    list_IP.erase(list_IP.begin() + id - 1);
    list_PORT.erase(list_PORT.begin() + id - 1);

    return;
}

void send(int id, std::string message) {//
    if (id > list_IP.size() - 1)
    {
        std::cout << "This connection does not exist try another";
        return;
    }
    // TODO:
    //       find the socket specified in the connection id parameter
    iResult = send(ConnectSocket, message.c_str(), (int)strlen(message.c_str()), 0);
    if (iResult == SOCKET_ERROR)
    {
        std::cout << "Message could not be sent: " << WSAGetLastError() << std::endl;
        closesocket(ConnectSocket);
        WSACleanup();
        return;
    }

    std::cout << "Message sent to " << id;

    return;
}

void exit() {
    // Make sure to update this to close all sockets in future
    closesocket(ConnectSocket);
    WSACleanup();
    return;
}

