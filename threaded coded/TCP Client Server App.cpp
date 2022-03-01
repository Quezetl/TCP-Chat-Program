#include<WinSock2.h>
#include "tsock.h"
#include <iostream>
#include <string>
#include <thread>
#include <chrono>

using namespace std;

int Run(string &response, string choice, tsock &App, bool quit);
int listening(tsock App)
{
    App.serverListen();
    return 0;
}


int main(int argc, char *argv[])
{
    string response;
    string choice;
    bool quit = false;
    int j;
    tsock App("27015");
    App.Server_init();
    thread shit(listening, App);
    do
    {
        getline(cin, response);
        choice = response.substr(0, response.find(' '));
        response.erase(0,choice.size()+1);
        Run(response, choice, App, quit);
        system("pause");
    } while (!quit);

    
    
}

int Run(string &response, string choice, tsock &App, bool quit)
{
    string first = response.substr(0, response.find(' '));
    response.erase(0, first.size()+1);
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
        App.Terminate();
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
        quit = true;
        return 1;
    }

    return 0;
}
