#ifndef SERVER_H
#define SERVER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <dirent.h>
#include <string>
#include <iostream>
#include <sstream>
#include "utils.h"

using namespace std;


struct Request {
	string method;
	string path;
	string version;
};

void runServer(int port, string dir);
void setUpServerSocket(int &sSocket, struct sockaddr_in &address, int port);
struct Request getRequest(int pSocket);
void respond(int pSocket, string version, string path, string reqPath);
string getContentType(string path);
string joinPath(string parent, string child);
string get404();
string getDirectory(string path, string fullPath);
void write(int pSocket, string msg);
void writeLine(int pSocket, string msg);
void writeLine(int pSocket);
void writeFile(int pSocket, string path);

#endif

