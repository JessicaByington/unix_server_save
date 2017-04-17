/*****************************************************************
 *  Author:         Jessica Maxey
 *  Filename:       main.cpp
 *  Date Created:   5/8/2016
 *  Modifications:
 *
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
 *      User will be ablet to specify the port and number of 
 *      clients it wants to allow, and be able to send a 
 *      string message across the server to other client. 
 *
 *  Output:
 *      Clients will recieve back the message that they typed out,
 *      as well as any other messages sent to the server.
 * **************************************************************/
#include <iostream>
using std::cin;

#include <unistd.h>
#include "server.h"

int main(int argc, char * argv[])
{
    int port = 0;
    int num_cli = 0;
    
    if(argc < 3)
    {
        fprintf(stderr, "Wrong number of args, defaulting port 4000, clients 20");
        port = 4000;
        num_cli = 20;
    }
    else
    {
        port = atoi(argv[1]);
        num_cli = atoi(argv[2]);
    }

    server s;
    
    s.StartServer(port, num_cli);
    
    while (1) 
    {
        string input;
        cin >> input;
        
        if (input == "stop")
        {
            s.StopServer();
            break;
        }
    }
    
    return 0;
}
