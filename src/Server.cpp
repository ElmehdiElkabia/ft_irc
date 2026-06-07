#include "Server.hpp"
#include <cerrno>
#include <cctype>


bool Server::Signal = false; //-> initialize the static boolean
void Server::SignalHandler(int signum) {
	(void)signum;
	std::cout << std::endl << "Signal Received!" << std::endl;
	Server::Signal = true; //-> set the static boolean to true to stop the server
}

void Server::CloseFds() {
	for(size_t i = 0; i < clients.size(); i++) { //-> close all the clients
		std::cout << RED << "Client <" << clients[i].GetFd() << "> Disconnected" << WHI << std::endl;
		close(clients[i].GetFd());
	}
	clients.clear();
	clientBuffers.clear();
	fds.clear();
	if (SerSocketFd != -1) { //-> close the server socket
		std::cout << RED << "Server <" << SerSocketFd << "> Disconnected" << WHI << std::endl;
		close(SerSocketFd);
		SerSocketFd = -1;
	}
}

Client *Server::FindClient(int fd) {
	for (size_t i = 0; i < clients.size(); ++i) {
		if (clients[i].GetFd() == fd)
			return &clients[i];
	}
	return 0;
}

void Server::DispatchCommand(int fd, const std::string &line) {
	if (line.empty())
		return;

	std::string commandLine = line;
	if (commandLine[0] == ':') {
		size_t prefixEnd = commandLine.find(' ');
		if (prefixEnd == std::string::npos)
			return;
		commandLine = commandLine.substr(prefixEnd + 1);
	}

	size_t commandEnd = commandLine.find(' ');
	std::string command = commandLine.substr(0, commandEnd);
	for (size_t i = 0; i < command.size(); ++i)
		command[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(command[i])));

	std::string params;
	if (commandEnd != std::string::npos)
		params = commandLine.substr(commandEnd + 1);

	if (!FindClient(fd))
		return;

	std::cout << YEL << "Client <" << fd << "> Command: " << WHI << command;
	if (!params.empty())
		std::cout << " | Args: " << params;
	std::cout << std::endl;
}

void Server::ProcessBuffer(int fd) {
	std::string &buffer = clientBuffers[fd];
	size_t lineEnd = std::string::npos;

	while ((lineEnd = buffer.find("\r\n")) != std::string::npos) {
		std::string line = buffer.substr(0, lineEnd);
		buffer.erase(0, lineEnd + 2);
		DispatchCommand(fd, line);
	}
}

void Server::SerSocket() {
	struct sockaddr_in add;
	struct pollfd NewPoll;
	add.sin_family = AF_INET; //-> set the address family to ipv4
	add.sin_port = htons(this->Port); //-> convert the port to network byte order (big endian)
	add.sin_addr.s_addr = INADDR_ANY; //-> set the address to any local machine address

	SerSocketFd = socket(AF_INET, SOCK_STREAM, 0); //-> create the server socket
	if(SerSocketFd == -1) //-> check if the socket is created
		throw(std::runtime_error("faild to create socket"));

	int en = 1;
	if(setsockopt(SerSocketFd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1) //-> set the socket option (SO_REUSEADDR) to reuse the address
		throw(std::runtime_error("faild to set option (SO_REUSEADDR) on socket"));
	if (fcntl(SerSocketFd, F_SETFL, O_NONBLOCK) == -1) //-> set the socket option (O_NONBLOCK) for non-blocking socket
		throw(std::runtime_error("faild to set option (O_NONBLOCK) on socket"));
	if (bind(SerSocketFd, (struct sockaddr *)&add, sizeof(add)) == -1) //-> bind the socket to the address
		throw(std::runtime_error("faild to bind socket"));
	if (listen(SerSocketFd, SOMAXCONN) == -1) //-> listen for incoming connections and making the socket a passive socket
		throw(std::runtime_error("listen() faild"));

	NewPoll.fd = SerSocketFd; //-> add the server socket to the pollfd
	NewPoll.events = POLLIN; //-> set the event to POLLIN for reading data
	NewPoll.revents = 0; //-> set the revents to 0
	fds.push_back(NewPoll); //-> add the server socket to the pollfd
}

void Server::AcceptNewClient() {
	Client cli; //-> create a new client
	struct sockaddr_in cliadd;
	struct pollfd NewPoll;
	socklen_t len = sizeof(cliadd);

	int incofd = accept(SerSocketFd, (sockaddr *)&(cliadd), &len); //-> accept the new client
	if (incofd == -1) {
		std::cout << "accept() failed" << std::endl;
		return;
	}

	if (fcntl(incofd, F_SETFL, O_NONBLOCK) == -1) { //-> set the socket option (O_NONBLOCK) for non-blocking socket 
		std::cout << "fcntl() failed" << std::endl;
		close(incofd);
		return;
	}

	NewPoll.fd = incofd; //-> add the client socket to the pollfd
	NewPoll.events = POLLIN; //-> set the event to POLLIN for reading data
	NewPoll.revents = 0; //-> set the revents to 0

	cli.SetFd(incofd); //-> set the client file descriptor
	cli.setIpAdd(inet_ntoa((cliadd.sin_addr))); //-> convert the ip address to string and set it
	clients.push_back(cli); //-> add the client to the vector of clients
	fds.push_back(NewPoll); //-> add the client socket to the pollfd

	std::cout << GRE << "Client <" << incofd << "> Connected" << WHI << std::endl;
}

void Server::ReceiveNewData(int fd) {
	char buff[1024]; //-> buffer for the received data
	memset(buff, 0, sizeof(buff)); //-> clear the buffer

	ssize_t bytes = recv(fd, buff, sizeof(buff) - 1 , 0); //-> receive the data
	if (bytes < 0) {
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return;
		std::cout << RED << "Client <" << fd << "> Disconnected" << WHI << std::endl;
		ClearClients(fd);
		return;
	}

	if(bytes <= 0) { //-> check if the client disconnected
		std::cout << RED << "Client <" << fd << "> Disconnected" << WHI << std::endl;
		ClearClients(fd); //-> clear the client
		return;
	}

	else { //-> print the received data
		buff[bytes] = '\0';
		clientBuffers[fd].append(buff, static_cast<size_t>(bytes));
		ProcessBuffer(fd);
	}
}

void Server::ServerInit() {
	this->Port = 4444;
	SerSocket(); //-> create the server socket

	std::cout << GRE << "Server <" << SerSocketFd << "> Connected" << WHI << std::endl;
	std::cout << "Waiting to accept a connection...\n";

	while (Server::Signal == false) //-> run the server until the signal is received
	{
		if((poll(&fds[0],fds.size(),-1) == -1) && Server::Signal == false) //-> wait for an event
			throw(std::runtime_error("poll() faild"));

		for (size_t i = 0; i < fds.size(); i++) //-> check all file descriptors
		{
			if (fds[i].revents & POLLIN)//-> check if there is data to read
			{
				if (fds[i].fd == SerSocketFd)
					AcceptNewClient(); //-> accept new client
				else
					ReceiveNewData(fds[i].fd); //-> receive new data from a registered client
			}
		}
	}
	CloseFds(); //-> close the file descriptors when the server stops
}
