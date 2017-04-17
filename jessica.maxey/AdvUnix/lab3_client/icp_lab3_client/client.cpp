/********************************************************************
 *  Author:         Jessica Maxey
 *  Filename:       client.cpp
 *  Date Create:    5/8/2016
 * *****************************************************************/
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <thread>
using std::thread;

#include <iostream>
using std::cout;
using std::endl;

#include "client.h"

int connect_socket = 0;
thread *read_thread = nullptr;
int first = 0;
bool invited = false;

/********************************************************************
 *  Purpose: create the client and connect to the server
 *
 *  Precondition:
 *      port and server to connect to needs to be specified
 *
 *  Postcondition:
 *      client will be connected/created
 *
 * *****************************************************************/
void CreateClient(string host_arg, int port, string name)
{
    addrinfo * res;
    addrinfo * result;
    addrinfo hint;
    int err;
    string connection_string;

    //set up hint structure 
    memset(&hint, 0, sizeof(hint));
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_canonname = NULL;
    hint.ai_addr = NULL;
    hint.ai_next = NULL;
    hint.ai_flags = AI_CANONNAME;

    //get server address
    if((err = getaddrinfo(host_arg.data(), "4000", NULL, &result)) != 0)
    {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(err));
        return;
    }
    
    //create socket 
    if((connect_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        fprintf(stderr, "socket error\n");
        return;
    }
    
    //set up the socket options
    int opt = 1;
    setsockopt(connect_socket, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(int));
    
    //check the results to make sure the correct type of family is there
    for (res = result; res != NULL; res = res->ai_next)
        if (res->ai_family == AF_INET)
            break;

    if (res != NULL)
    {
        //connect
        if(connect(connect_socket, res->ai_addr, res->ai_addrlen) < 0)
        {
            fprintf(stderr, "connect error\n");
            return;
        }
    } 

    connection_string = "new user connected: ";
    connection_string += name;
    WriteClient(connection_string);

    //start the reading thread
    read_thread = new thread(ReadClient);
}

/********************************************************************
 *  Purpose: Recieve messages from the server
 *
 *  Precondition:
 *      vaild client connected to server
 *
 *  Postcondition:
 *      messages will be read in 
 *
 * *****************************************************************/
void ReadClient()
{
    while(1)
    {
        char buff[256] = {0}; 
        int len = 0;

        if((len = recv(connect_socket, buff, 256, 0)) > 0)
        {
            string input = string(buff);
            cout << input;
            
            if(input.find("Join") != std::string::npos)
            {
                invited = true;
            }
            
        }
        else
        {
            close(connect_socket);
            connect_socket = 0;
            break;
        }
    }
}

/********************************************************************
 *  Purpose: Write messages out to other clients over the server
 *
 *  Precondition:
 *      vaild client connect to server
 *
 *  Postcondition:
 *      message will be send out to the server for other clients
 *      to recv
 * *****************************************************************/
void WriteClient(string input)
{
    string accept_string;

    if(invited == true)
    {
        invited = false;
        cout << "Invited = true\n";
        accept_string = "accept: ";
        accept_string += input;
        input = accept_string;
    }    

    input += "\r\n";
    auto ret = send(connect_socket, input.data(), input.size(), 0);


    if (ret < 0)
    {
        int err = errno;
        printf("Didn't send data, error: %d\n", err);
    }
}

/********************************************************************
 *  Purpose: Ends the connection to the server
 *
 *  Precondition:
 *      valid client connected to the server
 *
 *  Postcondition:
 *      client socket will be closed
 *
 * *****************************************************************/
void StopClient()
{
    close(connect_socket);
    connect_socket = 0;
}

/********************************************************************
 *  Purpose: check to see if client is connected
 *
 *  Precondition: n/a
 *
 *  Postcondition:
 *      returns if it is connected or not
 *
 * *****************************************************************/
bool ClientConnected()
{
    //It converts it into a bool
    return !!connect_socket;
}
