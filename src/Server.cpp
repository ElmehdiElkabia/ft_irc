#include "../include/Server.hpp"
#include "../include/Utils.hpp"


Server::Server()
{
}

Server::~Server()
{
}


Client* Server::getClientByNick(const std::string& nick)
{
    for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
    {
        if (it->second->getNickname() == nick)
            return it->second;
    }
    return nullptr;
}


void Server::handleCommand(Client* client, const std::string& commandLine)
{
    Command cmd = parseCommand(commandLine);

    if (cmd.command == "PASS")
        passCommand(client, cmd.params);
    else if (cmd.command == "NICK")
        nickCommand(client, cmd.params);
    else if (cmd.command == "USER")
        userCommand(client, cmd.params);
    else
        std::cout << "Unknown command: " << cmd.command << std::endl;

}