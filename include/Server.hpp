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
        std::map<int, Client*>             clients;
        std::map<std::string, Channel*>    channels;
        std::string                        password;

    public:
        Server();
        ~Server();

        // Client* getClient(int fd);
        // Client* getClientByNick(const std::string& nick);

        // Channel* getChannel(const std::string& name);

        // void addClient(Client* client);
        // void removeClient(int fd);

        // void addChannel(Channel* channel);

        void handleCommand(Client* client, const std::string& commandLine);

};

#endif