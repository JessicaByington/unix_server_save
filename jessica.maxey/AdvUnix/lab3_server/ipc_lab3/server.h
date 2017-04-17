/********************************************************************
 *  Author:         Jessica Maxey
 *  Filename:       server.h
 *  Date Created:   5/8/2016
 *  Modifications:
 * *****************************************************************/
#include <string>
using std::string;

#include <vector>
using std::vector;

#include <thread>
using std::thread;

#include <map>
using std::map;

#include <mutex>
using std::mutex;

#include <condition_variable>
using std::condition_variable;

struct Room
{
    string host_client;
    string invited_client;
    int index;
    int status;
    condition_variable cv;
    mutex cv_m;

    Room(){}
    Room(Room&& room) : 
        host_client(std::move(room.host_client)),
        invited_client(std::move(room.invited_client)),
        index(room.index),
        status(room.status),
        cv(),
        cv_m()
    {}
};

class server
{
    public:
    //ctor
    server();

    //dstructor
    ~server();
    void StartServer(int, int);
    void InviteClient(string, string);
    void StopServer();
    void CleanUp();
    
    
private: 
    int connect_socket;
    vector<Room> rooms;
    map<string, int> clients;
    vector<thread> client_threads;
    std::unique_ptr<thread> listen_thread;
    string user_name;
    mutex access_lock;
    bool done;
    void ListenServer();
    void ReadClient(int, string);
};
