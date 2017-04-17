/*****************************************************************
 *  Author:         Jessica Maxey
 *  Filename:       main.cpp
 *  Date Created:   5/8/2016
 *  Modifications:
 * ***************************************************************
 * ***************************************************************
 *  
 *  Lab/Assignment: Lab 03 - Interprocess Communication
 *
 *  Overview: 
 *      This program will use sockets to communicate through a
 *      server to other clients connect to it. It is a basic 
 *      chat program.
 *
 *  Input:
 *      User will be ablet to specify the port and the server 
 *      it wants to connect to, and be able to send a 
 *      string message across the server to other client. 
 *
 *  Output:
 *      Clients will recieve back the message that they typed out,
 *      as well as any other messages sent to the server.
 * **************************************************************/
#include <unistd.h>

#include <string>
using std::string;

#include <iostream>
using std::cin;
using std::cout;
using std::endl;

#include <thread>
using std::thread;

#include "client.h"
int port = 0;
string host_name;

void HandleConsole() 
{    
    while (1) 
    {
        string input;
        string user_name;

        getline(cin, input);
        
        if (input == "stop")
        {
            StopClient();
            break;
        }
        else if (input == "invite")
        {
            cout << "Enter username: ";
            getline(cin, user_name);
            
            input += ": ";
            input += user_name;
        } 
            
        WriteClient(input);
    }
}

int main (int argc, char ** argv)
{
    string input;
    string user_name;

    if(argc < 3)
    {
        fprintf(stderr, "Wrong number of args, exiting\n");
        return 0;
    }

    cout << "Welcome to battleship!\n";
    
    port = atoi(argv[1]);
    host_name = argv[2];

    cout << "Enter command: ";
    getline(cin, input);

    if(input == "join")
    {
        cout << "Enter username: ";
        getline(cin, user_name);

        CreateClient(host_name, port, user_name);
    
        auto console_thread = thread(HandleConsole);
    
        while (1)
            if (ClientConnected() == false)
                exit(0);
            else
               usleep(1000);
    }
    return 0;
}
