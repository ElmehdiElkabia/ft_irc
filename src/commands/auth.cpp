#include "../include/Server.hpp"

void Server::passCommand(Client *client, const std::vector<std::string> &params)
{
    if (params.size() != 1)
    {
        std::cerr << "Invalid number of parameters for PASS command." << std::endl;
        return;
    }
    if (client->hasPassed())
    {
        std::cerr << "You have already entered the password." << std::endl;
        return;
    }
    if (client->isRegistered())
    {
        std::cerr << "You are already registered." << std::endl;
        return;
    }
    std::string password = params[0];
    if (password != this->password)
    {
        std::cerr << "Incorrect password." << std::endl;
        return;
    }
    client->setPassAccepted(true);
}

