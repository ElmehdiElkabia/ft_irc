#include "../include/Server.hpp"
#include "../include/Utils.hpp"


Server::Server()
{
}

Server::~Server()
{
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