/********************************************************************
 *  Author:         Jessica Maxey
 *  Filename:       server.cpp
 *  Date Create:    5/8/2016
 * *****************************************************************/
#include <algorithm>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <chrono>
#include <string>
using std::to_string;

#include <vector>
using std::vector;


#include <thread>
#include <mutex>
using std::mutex;
using std::unique_lock;

#include <iostream>
using std::cout;

#include "server.h"

#define CONNECTED 0
#define INVITED 1
#define NOT_CONNECTED 2

server::server()
{}

server::~server()
{}

/********************************************************************
 *  Purpose: Create server socket
 *
 *  Preconditions:
 *      post and number of clients need to be provided
 *
 *  Postconditions:
 *      a vaild server and socket will be created and ready to 
 *      connect clients to
 *
 * *****************************************************************/
void server::StartServer(int port, int num_of_clients)
{
    sockaddr_in socket_info;
    int ret;
    done = false;
    
    connect_socket = socket(AF_INET, SOCK_STREAM, 6);
    
    if(connect_socket == -1)
    {
        perror("socket failed to connect");
        return;
    }
    
    socket_info.sin_addr.s_addr = INADDR_ANY;
    socket_info.sin_port = htons(port);
    socket_info.sin_family = AF_INET;
    
    //Sets options for the server sockets
    int opt = 1;
    //in case of error, it will reuse the addr and port of the socket
    setsockopt(connect_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(int));
    setsockopt(connect_socket, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(int));
    //it will also keep the socket alive
    setsockopt(connect_socket, SOL_SOCKET, SO_KEEPALIVE, &opt, sizeof(int));
    
    //bind the socket
    ret = bind(connect_socket, (const struct sockaddr *)&socket_info, sizeof(struct sockaddr_in));
    
    //check for errors with binding
    if (ret == -1)
    {
        perror("failed to bind");
        return;
    }

    //listen
    ret = listen(connect_socket, num_of_clients);
    
    //check the return value of listen for errors
    if(ret == -1)
    {
        perror("failed to listen");
        return;
    }
   
    //Calls the listenserver function to start listening
    //on another thread.
    auto proxy = [this]() 
    {
        this->ListenServer();
    };
    
    //reassigns the pointer to a new thread object
    //to start listening for client messages
    listen_thread.reset(new thread(proxy));    
}

/********************************************************************
 *  Purpose: Listens for connecting clients
 *
 *  Preconditions:
 *      vaild server must be created and bound
 *
 *  Postconditions:
 *      clients can connect
 *
 * *****************************************************************/
void server::ListenServer()
{
    while(!done)
    {
        sockaddr_in cli_addr = {0};
        char host[256] = {0};
        unsigned int cli_len = sizeof(cli_addr);
        int new_socket = 0;
        
        //accpets a new client socket        
        new_socket = accept(connect_socket, (sockaddr *)&cli_addr, &cli_len);
        
        //checks for error
        if(new_socket < 0)
        {
            fprintf(stderr, "Failed to accept\n");
            continue;
        }
        
        //gets the name from the client
        if(getnameinfo((sockaddr *)&cli_addr, sizeof(cli_addr), host, 
                    sizeof(host), NULL, 0, NI_NAMEREQD) != 0)
        {
            fprintf(stderr, "Failed to get name info\n");
            strcpy(host, inet_ntoa(cli_addr.sin_addr));
        }
    
        fprintf(stdout, "%s", host);   

        //check to make sure there is data from getnameinfo 
        if(host[0] != 0)
        {
            //search the map for the canonname to make sure 
            //dups are not added
            string name = host;
            
            access_lock.lock();
            auto it = clients.find(name);
            
            //find returns a end iterator if it was already in the map
            if (it != clients.end())
            {
                //char buff [] = "Already connected\n";
                
                //send message that is was already connected
                //send(new_socket, buff, sizeof(buff), 0);  
                //close(new_socket);
                
                name += to_string(clients.size());

                //access_lock.unlock();
                //continue;
            }
            
            //used to generate new thread for the client
            auto proxy = [this, new_socket, name]()
            {
                this->ReadClient(new_socket, name);
            };
            
            //add the new socket to the client map using the socket descriptor
            clients[name] = new_socket;
            //adds the new thread to the vector
            client_threads.emplace_back(proxy);
            
            if (client_threads.size() > 20)
                CleanUp();
                
            access_lock.unlock();
        }
    }
}

/********************************************************************
 *  Purpose: Read client messages and send them out to other clients
 *
 *  Preconditions:
 *      vaild server must be created and bound, vaild client must
 *      be connected
 *
 *  Postconditions:
 *      client can send and recieve messages
 *
 * *****************************************************************/
void server::ReadClient(int client_socket, string name)
{
    string id, user_name;
    char buff [512];
    int read_in = 0;

    if((read_in = read(client_socket, buff, sizeof(buff))) > 0)
    {
        buff[strcspn(buff, "\r")] = 0;

        string input = string(buff);

        size_t found = input.find_last_of(":");
        user_name = input.substr(found + 2);
    } 

    access_lock.lock();
    auto it = clients.find(name);
    auto user_it = clients.find(user_name);

    if (user_it != clients.end())
    {
        user_name += to_string(clients.size());
    }

    if (it != clients.end())
    {
        clients.erase(it);
        clients[user_name] = client_socket;
    }

    access_lock.unlock();
    
    id = user_name + ": ";
    
    //prints out the name for the user connecting
    fprintf(stdout, "User has joined: %s\n", user_name.data());
    
    //creates a string to be used for sending out the clients 
    //information over the send function.
    string user_joining = "User has joined: ";
    user_joining += user_name;
    user_joining += '\n';
    
    access_lock.lock();
    //iterates through all the connected clients and sends out 
    //that a new client has connected
    for(auto & it : clients)
    {
        send(it.second, user_joining.data(), user_joining.size(), 0);
    }
    access_lock.unlock();
        
    memset(buff, '\0', sizeof(buff));
    
    //read and write to the servver while the client and server are 
    //connected
    while (!done) 
    {    
        //reads in the clients message
        if((read_in = read(client_socket, buff, sizeof(buff))) > 0)
        {
            string input = string(buff);
         
            if(input.find("invite") != std::string::npos)
            {
                cout << user_name << " is inviting client\n";
                InviteClient(input, user_name);

            }
            else if(input.find("accept") != std::string::npos)
            {
                cout << "Accept check\n"; 
                bool accepted = false;
                
                if(input.find("y") != string::npos || input.find("Y") != string::npos) 
                {
                  accepted = true;
                }
                
                for (auto & room : rooms)
                {
                  if (room.status == INVITED && room.invited_client == user_name)
                  {
                    if (accepted)
                      room.status = CONNECTED;
                    else
                      room.status = NOT_CONNECTED; 
                      
                    room.cv.notify_one();
                  }
                  break;
                }

                /*
                //check to see if a invite has been sent
                for(unsigned int i = 0; i < rooms.size(); i++)
                {
                    if(rooms[i].status == INVITED && 
                            rooms[i].invited_client == user_name)
                    {
                        cout << "Found invited and user_name\n";

                        char response_buff [512];
                        while((read_in = read(client_socket, response_buff, 
                                        sizeof(response_buff))) > 0)
                        {
                        string r_input = string(response_buff);

                        const char * temp_r_buff = r_input.c_str();

                        int r_length = strlen(temp_r_buff);
                        char r_buff [r_length];
        
                        strcpy(r_buff, temp_r_buff);

                        r_buff[strcspn(r_buff, "\r")] = 0;
    
                        r_input.clear();

                        r_input = string(r_buff);
    
                        size_t found = r_input.find_last_of(":");
                        string response = r_input.substr(found + 2);
       
                        cout << "response = " <<  response << '\n';

                        if(response == "y" || response == "Y")
                        {
                            rooms[i].status = CONNECTED;
                            cout << "CONNECTED\n";
                            
                            string user_connected = "Joined";

                           send(user_it->second, user_connected.data(), 
                                   user_connected.size(), 0);
                            
                            r_input.clear();
                            input.clear();
                        }
                        else
                        {
                            rooms[i].status = NOT_CONNECTED;
                            cout << "NOT_CONNECTED\n";
                            r_input.clear();
                            input.clear();
                        }

                        rooms[i].cv.notify_one();
                        
                        memset(buff, '\0', sizeof(buff));
                        input.clear();
                        break;
                        
                        }
                        //memset(buff, '\0', sizeof(buff));
                        //input.clear();
                        //break;
                    }
                    //memset(buff, '\0', sizeof(buff));
                    //input.clear();
                    //break;
                }            
                //input.clear();
                //memset(buff, '\0', sizeof(buff));
                //break;
                */
            }
            else
            {
            access_lock.lock();


            //iterates through and sends it out to those connected
            //including the client who sent the message
            for(auto & it : clients)
            {
                send(it.second, id.data(), id.size(), 0);
                send(it.second, buff, read_in, 0);
            }
        
            memset(buff, '\0', sizeof(buff));

            access_lock.unlock();
            }
        
            memset(buff, '\0', sizeof(buff));
        }
        else
        {
            fprintf(stderr, "Client is leaving\n");

            //creating string to be used by send function
            //to send out that the user has left the chat room.
            string user_leaving = "User has left: ";                            
            user_leaving += user_name;
            user_leaving+= '\n';
            
            access_lock.lock();

            //make room struct
            //wait_for (semaphone)

            //close the socket
            close(client_socket);

            //remove from map
            clients.erase(user_name);

            //iterate through and send the leaving message
            for(auto & it : clients)
            {
                send(it.second, user_leaving.data(), user_leaving.size(), 0);
            }
                
            access_lock.unlock();
            break;
        }
    }
}

void server::InviteClient(string input, string current_user)
{
    string user_name;
    string invite_message;
    
    //sets up the input to just include the user
    //to invite

    cout << input << '\n';
    cout << user_name << '\n';
    
    const char * temp_i_buff = input.c_str();
    const char * temp_c_buff = current_user.c_str();

    int i_length = strlen(temp_i_buff);
    int c_length = strlen(temp_c_buff);

    char i_buff [i_length];
    char c_buff [c_length];
        
    strcpy(i_buff, temp_i_buff);
    strcpy(c_buff, temp_c_buff);

    i_buff[strcspn(i_buff, "\r")] = 0;
    c_buff[strcspn(c_buff, "\r")] = 0;
    
    input.clear();
    current_user.clear();

    input = string(i_buff);
    current_user = string(c_buff);

    size_t found = input.find_last_of(":");
    user_name = input.substr(found + 2);

    cout << input << '\n';
    cout << user_name << '\n';

    invite_message = current_user;
    invite_message += " wants to play a game. Join? (y/n): \r\n";

    access_lock.lock();
    for(auto const &ent1 : clients)
    {
        cout << "Client name: " << ent1.first << '\n';
    }

    auto it = clients.find(user_name);
    auto current_it = clients.find(current_user);

    access_lock.unlock();
    //look for that client
    if (it != clients.end())
    {
        //add current client and found client to rooms
        rooms.emplace_back();
        auto & room = rooms.back();
        
        room.host_client = current_user;
        room.invited_client = user_name;
        room.status = INVITED;
        room.index = rooms.size();
        //send a message to that saying they are being invited to play
        //set up game room?
        send(it->second, invite_message.data(), invite_message.size(), 0);
        
        //signal that we are done reading
        unique_lock<mutex> lock(room.cv_m);
        
        while(room.cv.wait_for(lock, std::chrono::seconds(1)) == 
            std::cv_status::timeout){}

        //if yes, then they are now connected
        if(room.status == CONNECTED)
        {
            string accept_message = user_name;
            accept_message += " has accepted your game\n";

            //send message back to requestee
            send(current_it->second, accept_message.data(), 
                    accept_message.size(), 0);

            accept_message.clear();
            accept_message = "connected";

            send(current_it->second, accept_message.data(), 
                    accept_message.size(), 0);
            
            send(it->second, accept_message.data(), 
                    accept_message.size(), 0);
        }
        else
        {
            string rejection_message = user_name;
            rejection_message += " has rejected your game\n";
            
            //send message back to requestee
            send(current_it->second, rejection_message.data(), 
                    rejection_message.size(), 0);
            rooms.pop_back();
        }

    }
    else
    {
        //send to current user that that user
        //does not exist
        string non_exist_message = user_name;
        non_exist_message += " does not exists\n";
        
        send(current_it->second, non_exist_message.data(), 
                non_exist_message.size(), 0);
    }
    
}

/********************************************************************
 *  Purpose: To stop the server, does the closing of the sockets
 *      and clearing of the clients
 *
 *  Preconditions:
 *      vaild server has been created and bound, and the stop
 *      command has been entered
 *
 *  Postconditions:
 *
 * *****************************************************************/
void server::StopServer()
{
    done = true;
    char stop_message [] = "Server shutting down\n";
    
    access_lock.lock();

    //tells all the clients that are connected that the server
    //is shutting down
    for(auto & it : clients)
    {
        send(it.second, stop_message, sizeof(stop_message), 0);
        shutdown(it.second, SHUT_RD);
    }
    access_lock.unlock();
    
    //joins all the clinet threads
    for(auto & it : client_threads)
    {
        it.join();
    }
    
    //Stops allowing clients to read and write to/from the server
    //and shuts the sockets down
    shutdown(connect_socket, SHUT_RDWR);
    close(connect_socket);
    
    //clears the map
    clients.clear();
    //clears the client threads
    client_threads.clear();
    
    //joins the threads listening for client messages
    listen_thread->join();
}

/********************************************************************
 *  Purpose: Clean up client threads while running, to keep the client
 *      count low
 *
 *  Preconditions:
 *      client count goes over 20
 *
 *  Postconditions:
 *      clients will be removed
 *
 * *****************************************************************/
 void server::CleanUp()
 {
    //Cleans up client threads that are ready to be joined
    auto check_joinable = [](thread &t) -> bool
    {
        if (t.joinable())
        {
            t.join();
            return true;
        }   
        
        return false;
    };
 
    //the clean up call, that uses the above function. It uses a iterator 
    //to call the check_joinable function, and if it is joinable then it 
    //eases it from the vector
    client_threads.erase (std::remove_if(client_threads.begin(), client_threads.end(), check_joinable), client_threads.end());    
 }
