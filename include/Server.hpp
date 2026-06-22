#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"
#include "Channel.hpp"

#include <iostream>
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <map>
#include <vector>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>
#include <csignal>

#define RED "\e[1;31m"
#define WHI "\e[0;37m"
#define GRE "\e[1;32m"
#define YEL "\e[1;33m"

class Client;
class Channel;

class Server
{
private:
    // Network
    int Port;
    int SerSocketFd;
	static volatile sig_atomic_t Signal; //-> static signal flag

    std::vector<struct pollfd> fds;
    std::map<int, std::string> clientBuffers;

    Client *FindClient(int fd);
    void ProcessBuffer(int fd);
    void DispatchCommand(int fd, const std::string &line);

    // IRC data
    std::map<int, Client *> clients;
    std::map<std::string, Channel *> channels;
    std::string password;

public:
    Server();
    ~Server();

    // Client* getClient(int fd);

    // Network
    void ServerInit(  int &port, std::string &password );
    void SerSocket();
    void AcceptNewClient();
    void ReceiveNewData(int fd);

    static void SignalHandler(int signum);

    void CloseFds();
    void ClearClients(int fd);

    // IRC
    Client *getClientByNick(const std::string &nick);
    Channel *getChannel(const std::string &name);

    void addClient(Client *client);
    // void removeClient(int fd);

    // void addChannel(Channel* channel);

    void handleCommand(Client *client, const std::string &commandLine);

    void passCommand(Client *client, const std::vector<std::string> &params);
    void nickCommand(Client *client, const std::vector<std::string> &params);
    void userCommand(Client *client, const std::vector<std::string> &params);
    void joinCommand(Client *client, const std::vector<std::string> &params);
    void partCommand(Client *client, const std::vector<std::string> &params);
    void quitCommand(Client *client, const std::vector<std::string> &params);
    void privmsgCommand(Client *client, const std::vector<std::string> &params);
    void topicCommand(Client *client, const std::vector<std::string> &params);
    void inviteCommand(Client *client, const std::vector<std::string> &params);
    void kickCommand(Client *client, const std::vector<std::string> &params);
    void modeCommand(Client *client, const std::vector<std::string> &params);

    void sendToClient(Client *client, const std::string &message);
};

#endif