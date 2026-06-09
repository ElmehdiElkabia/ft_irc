#include "../../include/Server.hpp"

void Server::privmsgCommand(Client *client, const std::vector<std::string> &params)
{
    if (!client->isRegistered())
    {
        std::cerr << "You must be registered to send a private message." << std::endl;
        return;
    }
    if (params.size() < 2)
    {
        std::cerr << "Invalid number of parameters for PRIVMSG command." << std::endl;
        return;
    }
    std::string target = params[0];
    std::string message = params[1];
    if (target[0] == '#')
    {
        Channel *channel = getChannel(target);
        if (!channel)
        {
            std::cerr << "No such channel: " << target << std::endl;
            return;
        }
        if (!channel->isMember(client))
        {
            std::cerr << "You are not a member of channel: " << target << std::endl;
            return;
        }
        for (size_t i = 0; i < channel->memberCount(); ++i)
        {
            Client *member = channel->getMembers()[i];

            if (member != client)
                std::cout << "Sending message to " << member->getNickname() << ": " << message << std::endl;
        }
    }
    else
    {
        Client *targetClient = getClientByNick(target);
        if (!targetClient)
        {
            std::cerr << "No such user: " << target << std::endl;
            return;
        }
        std::cout << "Sending private message to " << targetClient->getNickname() << ": " << message << std::endl;
    }
}