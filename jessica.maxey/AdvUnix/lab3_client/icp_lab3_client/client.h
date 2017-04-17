/********************************************************************
 *  Author:         Jessica Maxey
 *  Filename:       client.h
 *  Date Created:   5/8/2016
 *
 * *****************************************************************/
#include <string>
using std::string;

void CreateClient(string host_arg, int port, string name);
void ReadClient();
void WriteClient(string);
void StopClient();
bool ClientConnected();
