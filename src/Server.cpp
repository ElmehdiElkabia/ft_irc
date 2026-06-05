#include "../include/Server.hpp"
#include "../include/Command.hpp"


Server::Server()
{
}

Server::~Server()
{
}



void Server::handleCommand(Client* client, const std::string& commandLine)
{
    Command cmd = parseCommand(commandLine);
}