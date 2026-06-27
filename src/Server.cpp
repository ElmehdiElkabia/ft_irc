#include "../include/Server.hpp"
#include "../include/Utils.hpp"

#include <cerrno>
#include <cctype>

volatile sig_atomic_t Server::Signal = 0; //-> initialize the static signal flag
void Server::SignalHandler(int signum)
{
	(void)signum;
	Server::Signal = 1; //-> set the static flag to stop the server
}

void Server::CloseFds()
{
	for (std::map<int, Client *>::iterator it = clients.begin();
		 it != clients.end();
		 ++it)
	{
		std::cout << RED << "Client <" << it->first << "> Disconnected" << WHI << std::endl;

		std::string errorMsg = "ERROR :Server shutting down (Ctrl+C received)\r\n";
		send(it->first, errorMsg.c_str(), errorMsg.length(), 0);

		shutdown(it->first, SHUT_RDWR);
		close(it->first);
		delete it->second;
	}

	clients.clear();
	clientBuffers.clear();
	fds.clear();
	if (SerSocketFd != -1)
	{ //-> close the server socket
		std::cout << RED << "Server <" << SerSocketFd << "> Disconnected" << WHI << std::endl;
		close(SerSocketFd);
		SerSocketFd = -1;
	}
}

Client *Server::FindClient(int fd)
{
	std::map<int, Client *>::iterator it = clients.find(fd);

	if (it != clients.end())
		return it->second;

	return NULL;
}
void Server::DispatchCommand(int fd, const std::string &line)
{
	if (line.empty())
		return;

	std::string commandLine = line;
	if (commandLine[0] == ':')
	{
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

	Client *client = FindClient(fd);

	if (!client)
		return;

	handleCommand(client, line);

	// if (!params.empty())
	// 	std::cout << " | Args: " << params;
	// std::cout << std::endl;
}

// void Server::ProcessBuffer(int fd)
// {
// 	std::string &buffer = clientBuffers[fd];
// 	size_t lineEnd = std::string::npos;

// 	while ((lineEnd = buffer.find("\r\n")) != std::string::npos)
// 	{
// 		std::string line = buffer.substr(0, lineEnd);
// 		buffer.erase(0, lineEnd + 2);
// 		DispatchCommand(fd, line);
// 	}
// }

void Server::ProcessBuffer(int fd)
{
	std::string &buffer = clientBuffers[fd];
	size_t lineEnd;

	while ((lineEnd = buffer.find('\n')) != std::string::npos)
	{
		std::string line = buffer.substr(0, lineEnd);

		if (!line.empty() && line[line.size() - 1] == '\r')
			line.erase(line.size() - 1);

		buffer.erase(0, lineEnd + 1);

		DispatchCommand(fd, line);
	}
}

void Server::SerSocket()
{
	struct sockaddr_in add;
	struct pollfd NewPoll;
	add.sin_family = AF_INET;		  //-> set the address family to ipv4
	add.sin_port = htons(this->Port); //-> convert the port to network byte order (big endian)
	add.sin_addr.s_addr = INADDR_ANY; //-> set the address to any local machine address

	SerSocketFd = socket(AF_INET, SOCK_STREAM, 0); //-> create the server socket
	if (SerSocketFd == -1)						   //-> check if the socket is created
		throw(std::runtime_error("faild to create socket"));

	int en = 1;
	if (setsockopt(SerSocketFd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1) //-> set the socket option (SO_REUSEADDR) to reuse the address
		throw(std::runtime_error("faild to set option (SO_REUSEADDR) on socket"));
	if (fcntl(SerSocketFd, F_SETFL, O_NONBLOCK) == -1) //-> set the socket option (O_NONBLOCK) for non-blocking socket
		throw(std::runtime_error("faild to set option (O_NONBLOCK) on socket"));
	if (bind(SerSocketFd, (struct sockaddr *)&add, sizeof(add)) == -1) //-> bind the socket to the address
		throw(std::runtime_error("faild to bind socket"));
	if (listen(SerSocketFd, SOMAXCONN) == -1) //-> listen for incoming connections and making the socket a passive socket
		throw(std::runtime_error("listen() faild"));

	NewPoll.fd = SerSocketFd; //-> add the server socket to the pollfd
	NewPoll.events = POLLIN;  //-> set the event to POLLIN for reading data
	NewPoll.revents = 0;	  //-> set the revents to 0
	fds.push_back(NewPoll);	  //-> add the server socket to the pollfd
}

void Server::AcceptNewClient()
{
	Client *cli = new Client(); //-> create a new client
	struct sockaddr_in cliadd;
	struct pollfd NewPoll;
	socklen_t len = sizeof(cliadd);

	int incofd = accept(SerSocketFd, (sockaddr *)&(cliadd), &len); //-> accept the new client
	if (incofd == -1)
	{
		std::cout << "accept() failed" << std::endl;
		return;
	}

	if (fcntl(incofd, F_SETFL, O_NONBLOCK) == -1)
	{ //-> set the socket option (O_NONBLOCK) for non-blocking socket
		std::cout << "fcntl() failed" << std::endl;
		close(incofd);
		return;
	}

	NewPoll.fd = incofd;	 //-> add the client socket to the pollfd
	NewPoll.events = POLLIN; //-> set the event to POLLIN for reading data
	NewPoll.revents = 0;	 //-> set the revents to 0

	cli->setFd(incofd);							 //-> set the client file descriptor
	cli->setIpAdd(inet_ntoa((cliadd.sin_addr))); //-> convert the ip address to string and set it
	clients[incofd] = cli;						 //-> add the client to the map of clients
	fds.push_back(NewPoll);						 //-> add the client socket to the pollfd

	std::cout << GRE << "Client <" << incofd << "> Connected" << WHI << std::endl;
}

void Server::ReceiveNewData(int fd)
{
	char buff[1024];			   //-> buffer for the received data
	memset(buff, 0, sizeof(buff)); //-> clear the buffer

	ssize_t bytes = recv(fd, buff, sizeof(buff) - 1, 0); //-> receive the data
	if (bytes < 0)
	{
		if (errno == EAGAIN || errno == EWOULDBLOCK)
			return;
		std::cout << RED << "Client <" << fd << "> Disconnected" << WHI << std::endl;
		ClearClients(fd);
		return;
	}

	if (bytes <= 0)
	{ //-> check if the client disconnected
		std::cout << RED << "Client <" << fd << "> Disconnected" << WHI << std::endl;
		ClearClients(fd); //-> clear the client
		return;
	}

	else
	{ //-> print the received data
		buff[bytes] = '\0';
		clientBuffers[fd].append(buff, static_cast<size_t>(bytes));
		// std::cout << "Received: [" << buff << "]" << std::endl;
		ProcessBuffer(fd);
	}
}

void Server::ServerInit(int &port, std::string &password)
{
	this->Port = port;
	this->password = password;

	SerSocket(); //-> create the server socket

	std::cout << GRE << "Server <" << SerSocketFd << "> Connected" << WHI << std::endl;
	std::cout << "Waiting to accept a connection...\n";

	while (Server::Signal == 0) //-> run the server until the signal is received
	{
		if ((poll(&fds[0], fds.size(), -1) == -1))
		{
			if (Server::Signal == 1)
				break; // Stop immediately if Ctrl+C was pressed
			throw(std::runtime_error("poll() failed"));
		}

		for (size_t i = 0; i < fds.size(); i++) //-> check all file descriptors
		{
			// 1. Handle errors or hang-ups first
			if (fds[i].revents & (POLLHUP | POLLERR | POLLNVAL))
			{
				if (fds[i].fd != SerSocketFd)
				{
					ClearClients(fds[i].fd);
					i--; // ---> FIX: Decrement index because ClearClients removed an item from fds <---
				}
				continue;
			}

			// 2. Handle data reading
			if (fds[i].revents & POLLIN) //-> check if there is data to read
			{
				if (fds[i].fd == SerSocketFd)
					AcceptNewClient(); // Adds to fds, which is fine since size increases at the end
				else
				{
					ReceiveNewData(fds[i].fd);
					if (i < fds.size() && fds[i].revents == 0)
						i--;
				}
			}
		}
	}
	CloseFds();
}

void Server::ClearClients(int fd)
{
	close(fd);

	for (size_t i = 0; i < fds.size(); i++)
	{
		if (fds[i].fd == fd)
		{
			fds.erase(fds.begin() + i);
			break;
		}
	}

	std::map<int, Client *>::iterator it = clients.find(fd);

	if (it != clients.end())
	{
		delete it->second;
		clients.erase(it);
	}

	clientBuffers.erase(fd);
}

Server::Server()
{
	this->password = "pass";
	this->SerSocketFd = -1;
}

Server::~Server()
{
	for (std::map<std::string, Channel *>::iterator it = channels.begin();
		 it != channels.end();
		 ++it)
	{
		delete it->second;
	}

	channels.clear();
}

// Client *Server::getClientByNick(const std::string &nick)
// {
// 	for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
// 	{
// 		if (it->second->getNickname() == nick)
// 			return it->second;
// 	}
// 	return NULL;
// }

Client *Server::getClientByNick(const std::string &nick)
{
	std::cout << "Searching for [" << nick << "]\n";

	for (std::map<int, Client *>::iterator it = clients.begin();
		 it != clients.end();
		 ++it)
	{
		std::cout
			<< "fd="
			<< it->first
			<< " nick=["
			<< it->second->getNickname()
			<< "]"
			<< std::endl;

		if (it->second->getNickname() == nick)
			return it->second;
	}

	return NULL;
}

Channel *Server::getChannel(const std::string &name)
{
	std::map<std::string, Channel *>::iterator it = channels.find(name);
	if (it != channels.end())
		return it->second;
	return NULL;
}

void Server::handleCommand(Client *client, const std::string &commandLine)
{
	Command cmd = parseCommand(commandLine);

	if (cmd.command == "PASS")
		passCommand(client, cmd.params);
	else if (cmd.command == "NICK")
		nickCommand(client, cmd.params);
	else if (cmd.command == "USER")
		userCommand(client, cmd.params);
	else if (cmd.command == "JOIN")
		joinCommand(client, cmd.params);
	else if (cmd.command == "PART")
		partCommand(client, cmd.params);
	else if (cmd.command == "PRIVMSG")
		privmsgCommand(client, cmd.params);
	else if (cmd.command == "TOPIC")
		topicCommand(client, cmd.params);
	else if (cmd.command == "INVITE")
		inviteCommand(client, cmd.params);
	else if (cmd.command == "KICK")
		kickCommand(client, cmd.params);
	else if (cmd.command == "MODE")
		modeCommand(client, cmd.params);
	else
		sendToClient(client, ERR_UNKNOWNCOMMAND(client->getNickname(), cmd.command) + "\r\n");
}

void Server::addClient(Client *client)
{
	clients[client->getFd()] = client;
}

void Server::sendToClient(Client *client, const std::string &message)
{
	send(client->getFd(),
		 message.c_str(),
		 message.size(),
		 0);
}

void    Server::setBot( const Bot &bot )
{
	_bot = bot;
}

const Bot    &Server::getBot( void ) const
{
	return (_bot);
}

int       Server::clientSize( void ) const
{
	int										_size = 0;
	std::map<int, Client *>::const_iterator	it = clients.begin();

	while ( it != clients.end() )
	{
		_size++;
		it++;
	}

	return (_size);
}

int       Server::channelSize( void ) const
{
	int													_size = 0;
	std::map<std::string, Channel *>::const_iterator	it = channels.begin();

	while ( it != channels.end() )
	{
		_size++;
		it++;
	}

	return (_size);
}

const std::string   Server::clientsName( void ) const
{
	std::string								names;
	std::map<int, Client *>::const_iterator	it = clients.begin();

	Client	client;
	while ( it != clients.end() )
	{
		names += it->second->getNickname();
		names += "\n";
		it++;
	}

	return (names);
}

const std::string   Server::channelsName( void ) const
{
	std::string											channel_names;
	std::map<std::string, Channel *>::const_iterator	it = channels.begin();

	// std::cout << "===== MAP ORDER =====" << std::endl;

	// for (std::map<std::string, Channel*>::const_iterator it = channels.begin(); it != channels.end(); ++it)
	// {
	//     std::cout << "[" << it->first << "]" << std::endl;
	// }

	// std::cout << "=====================" << std::endl;

	while ( it != channels.end() )
	{
		channel_names += it->first;
		channel_names += "\n";
		it++;
	}

	return (channel_names);
}
