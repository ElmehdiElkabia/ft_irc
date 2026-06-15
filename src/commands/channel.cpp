#include "../../include/Server.hpp"

void Server::joinCommand(Client *client,
                         const std::vector<std::string> &params)
{
    if (!client->isRegistered())
    {
        sendToClient(client, "ERROR :You must be registered to join a channel\r\n");

        return;
    }

    if (params.size() < 1 || params.size() > 2)
    {
        sendToClient(client, "ERROR :Invalid number of parameters for JOIN command\r\n");
        return;
    }

    std::string channelName = params[0];

    if (channelName.empty() || channelName[0] != '#')
    {
        sendToClient(client, "ERROR :Invalid channel name. Channel names must start with '#'.\r\n");
        return;
    }

    Channel *channel = getChannel(channelName);

    if (!channel)
    {
        channel = new Channel(channelName);
        channels[channelName] = channel;

        channel->addMember(client);
        channel->addOperator(client);

        sendToClient(client, "Client " + client->getNickname() + " created channel " + channelName + " and became operator." + "\r\n");
        return;
    }

    if (channel->isMember(client))
    {
        sendToClient(client, "ERROR :You are already a member of channel: " + channelName + "\r\n");
        return;
    }

    if (channel->isInviteOnly())
    {
        if (!channel->isInvited(client))
        {
            sendToClient(client, "ERROR :Channel " + channelName + " is invite-only. You must be invited to join.\r\n");
            return;
        }

        channel->removeInvitation(client);
    }

    if (channel->hasKey())
    {
        if (params.size() < 2)
        {
            sendToClient(client, "ERROR :Channel " + channelName + " requires a password. Please provide the password to join.\r\n");
            return;
        }

        if (params[1] != channel->getKey())
        {
            sendToClient(client, "ERROR :Incorrect channel password for channel " + channelName + ".\r\n");
            return;
        }
    }

    if (channel->getUserLimit() > 0)
    {
        if (channel->memberCount() >= static_cast<size_t>(channel->getUserLimit()))
        {
            sendToClient(client, "ERROR :Channel " + channelName + " is full. User limit reached.\r\n");
            return;
        }
    }

    channel->addMember(client);
    sendToClient(client, "Client " + client->getNickname() + " joined channel " + channelName + "." + "\r\n");
}

void Server::partCommand(Client *client, const std::vector<std::string> &params)
{
    if (!client->isRegistered())
    {
        sendToClient(client, "ERROR :You must be registered to part a channel\r\n");
        return;
    }
    if (params.size() != 1)
    {
        sendToClient(client, "ERROR :Invalid number of parameters for PART command\r\n");
        return;
    }
    std::string channelName = params[0];
    if (channelName[0] != '#')
    {
        sendToClient(client, "ERROR :Invalid channel name. Channel names must start with '#'.\r\n");
        return;
    }
    Channel *channel = getChannel(channelName);
    if (!channel)
    {
        sendToClient(client, "ERROR :No such channel: " + channelName + "\r\n");
        return;
    }
    if (!channel->isMember(client))
    {
        sendToClient(client, "ERROR :You are not a member of channel: " + channelName + "\r\n");
        return;
    }
    channel->removeMember(client);
    if (channel->memberCount() == 1)
    {
        delete channel;
        channels.erase(channelName);
        sendToClient(client, "Channel " + channelName + " has been deleted as it has no more members.\r\n");
        return;
    }
    sendToClient(client, "You have left channel: " + channelName + "\r\n");
}

void Server::quitCommand(Client *client, const std::vector<std::string> &params)
{

    (void)params;
    // you check if the client is registerd
    // you remove the client from all channels they are a member
    // you delete the client from the server's client list
    // you should close the socket of the client
    // okay a khay abdo
    // rah dart likk koulchii hena
    sendToClient(client, "You are quitting the IRC server.\r\n");
}