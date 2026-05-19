#pragma once

#include "Client.hpp"
#include <iostream>
#include <vector> //-> for vector
#include <sys/socket.h> //-> for socket()
#include <sys/types.h> //-> for socket()
#include <netinet/in.h> //-> for sockaddr_in
#include <fcntl.h> //-> for fcntl()
#include <unistd.h> //-> for close()
#include <arpa/inet.h> //-> for inet_ntoa()
#include <poll.h> //-> for poll()
#include <csignal> //-> for signal()
//-------------------------------------------------------//
#define RED "\e[1;31m" //-> for red color
#define WHI "\e[0;37m" //-> for white color
#define GRE "\e[1;32m" //-> for green color
#define YEL "\e[1;33m" //-> for yellow color
//-------------------------------------------------------//

class Server //-> class for server
{
private:
	int Port; //-> server port
	int SerSocketFd; //-> server socket file descriptor
	static bool Signal; //-> static boolean for signal
	std::vector<Client> clients; //-> vector of clients
	std::vector<struct pollfd> fds; //-> vector of pollfd
public:
	Server(){SerSocketFd = -1;} //-> default constructor

	void ServerInit(); //-> server initialization
	void SerSocket(); //-> server socket creation
	void AcceptNewClient(); //-> accept new client
	void ReceiveNewData(int fd); //-> receive new data from a registered client

	static void SignalHandler(int signum); //-> signal handler
 
	void CloseFds(); //-> close file descriptors
	void ClearClients(int fd); //-> clear clients
};

//-------------------------------------------------------//
void Server::ClearClients(int fd) { //-> clear the clients
    close(fd);
	for(size_t i = 0; i < fds.size(); i++) { //-> remove the client from the pollfd
		if (fds[i].fd == fd) {
            fds.erase(fds.begin() + i);
            break;
        }
	}
	for(size_t i = 0; i < clients.size(); i++) { //-> remove the client from the vector of clients
		if (clients[i].GetFd() == fd) {
			clients.erase(clients.begin() + i);
            break;
        }
	}
}

