#ifndef SERVER_HPP
#define SERVER_HPP

#include "Client.hpp"
#include "Channel.hpp"
#include <iostream>
#include <map>

class Client;
class Channel;

class Server
{
private:
    std::map<int, Client *> clients;
    std::map<std::string, Channel *> channels;
    std::string password;

public:
    Server();
    ~Server();

    // Client* getClient(int fd);
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
    void quitCommnand(Client *client, const std::vector<std::string> &params);
    void privmsgCommand(Client *client, const std::vector<std::string> &params);
    void topicCommand(Client *client, const std::vector<std::string> &params);
    void inviteCommand(Client *client, const std::vector<std::string> &params);
    void kickCommand(Client *client, const std::vector<std::string> &params);
};

#endif