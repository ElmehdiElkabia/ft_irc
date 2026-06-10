#include "../../include/Server.hpp"

void Server::privmsgCommand(Client *client, const std::vector<std::string> &params)
{
    if (!client->isRegistered())
    {
        sendToClient(client, "ERROR :You must be registered to send a private message\r\n");
        return;
    }
    if (params.size() < 2)
    {
        sendToClient(client, "ERROR :Invalid number of parameters for PRIVMSG command\r\n");
        return;
    }
    std::string target = params[0];
    std::string message = params[1];
    if (target[0] == '#')
    {
        Channel *channel = getChannel(target);
        if (!channel)
        {
            sendToClient(client, "ERROR :No such channel: " + target + "\r\n");
            return;
        }
        if (!channel->isMember(client))
        {
            sendToClient(client, "ERROR :You are not a member of channel: " + target + "\r\n");
            return;
        }
        for (size_t i = 0; i < channel->memberCount(); ++i)
        {
            Client *member = channel->getMembers()[i];

            if (member != client)
                sendToClient(member, "PRIVMSG " + target + " :" + message + "\r\n");
        }
    }
    else
    {
        Client *targetClient = getClientByNick(target);
        if (!targetClient)
        {
            sendToClient(client, "ERROR :No such user: " + target + "\r\n");
            return;
        }
        sendToClient(targetClient, "PRIVMSG " + target + " :" + message + "\r\n");
    }
}