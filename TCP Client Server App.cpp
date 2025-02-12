#include<WinSock2.h>
#include "tsock.h"
#include <iostream>
#include <string>
#include <thread>

using namespace std;

int Run(string& response, string choice, tsock& App);

int main(int argc, char* argv[])
{
    tsock App(argv[1]);
    string response;
    string choice;
    bool quit = false;
    App.Server_init();
    App.serverListen();
    thread worker1(&tsock::serverAccept, &App);
    thread worker2(&tsock::serverAccept, &App);

    while (!quit)
    {
        getline(cin, response);
        choice = response.substr(0, response.find(' '));
        response.erase(0, choice.size() + 1);
        quit = Run(response, choice, App);
        
        terminate();
    }

    worker1.join();
    worker2.join();
    return 0;
}

int Run(string& response, string choice, tsock& App)
{
    string first = response.substr(0, response.find(' '));
    response.erase(0, first.size() + 1);
    if (choice == "accept")
        App.serverAccept();
    if (choice == "help")
    {
        App.Help();
        return 0;
    }
    else if (choice == "myip")
    {
        App.Myip();
        return 0;
    }
    else if (choice == "myport")
    {
        App.Myport();
        return 0;
    }
    else if (choice == "connect")
    {
        App.Connect(first, response);
        return 0;
    }
    else if (choice == "list")
    {
        App.List();
        return 0;
    }
    else if (choice == "terminate")
    {
        App.Terminate(atoi(first.c_str()));
        return 0;
    }
    else if (choice == "send")
    {
        App.Send(atoi(first.c_str()), response);
        return 0;
    }
    else if (choice == "exit")
    {
        App.Exit();
        return 1;
    }

    return 0;
}