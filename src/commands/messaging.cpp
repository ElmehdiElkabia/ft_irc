#include "../../include/Server.hpp"

void Server::privmsgCommand(Client *client, const std::vector<std::string> &params)
{
    if (!client->isRegistered())
    {
        sendToClient(client, ERR_NOTREGISTERED(client->getNickname()) + "\r\n");
        return;
    }
    if (params.size() < 2)
    {
        sendToClient(client, ERR_NEEDMOREPARAMS(client->getNickname(), "PRIVMSG") + "\r\n");
        return;
    }
    std::string target  = params[0];
    std::string message = params[1];

    if (message.empty())
    {
        sendToClient(client, ERR_NOTEXTTOSEND(client->getNickname()) + "\r\n");
        return;
    }

    std::string prefix = ":" + client->getPrefix(); // nick!user@host

    if (target[0] == '#')
    {
        Channel *channel = getChannel(target);
        if (!channel)
        {
            sendToClient(client, ERR_NOSUCHCHANNEL(client->getNickname(), target) + "\r\n");
            return;
        }
        if (!channel->isMember(client))
        {
            sendToClient(client, ERR_CANNOTSENDTOCHAN(client->getNickname(), target) + "\r\n");
            return;
        }
        for (size_t i = 0; i < channel->memberCount(); ++i)
        {
            Client *member = channel->getMembers()[i];
            if (member != client)
                sendToClient(member, prefix + " PRIVMSG " + target + " :" + message + "\r\n");
        }
    }
    else
    {
        Client *targetClient = getClientByNick(target);
        if (!targetClient)
        {
            sendToClient(client, ERR_NOSUCHNICK(client->getNickname(), target) + "\r\n");
            return;
        }
        sendToClient(targetClient, prefix + " PRIVMSG " + target + " :" + message + "\r\n");
    }
}