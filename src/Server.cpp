#include "../include/Server.hpp"
#include "../include/Utils.hpp"

Server::Server()
{
    this->password = "secret";
}

Server::~Server()
{
}

Client *Server::getClientByNick(const std::string &nick)
{
    for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
    {
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
    else if (cmd.command == "QUIT")
        quitCommnand(client, cmd.params);
    else if (cmd.command == "PRIVMSG")
        privmsgCommand(client, cmd.params);
    else if(cmd.command == "TOPIC")
        topicCommand(client, cmd.params);
	else if (cmd.command == "INVITE")
		inviteCommand(client, cmd.params);
    else if (cmd.command == "KICK")
        kickCommand(client, cmd.params);
    else
        std::cout << "Unknown command: " << cmd.command << std::endl;
}

void Server::addClient(Client *client)
{
    clients[client->getFd()] = client;
}