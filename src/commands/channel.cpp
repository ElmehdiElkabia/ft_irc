#include "../../include/Server.hpp"

void Server::joinCommand(Client *client,
                         const std::vector<std::string> &params)
{
    if (!client->isRegistered())
    {
        sendToClient(client,
                     ERR_NOTREGISTERED(client->getNickname()) + "\r\n");
        return;
    }

    if (params.empty())
    {
        sendToClient(client,
                     ERR_NEEDMOREPARAMS(client->getNickname(), "JOIN") + "\r\n");
        return;
    }

    std::string channelName = params[0];

    if (channelName.empty() || channelName[0] != '#')
    {
        sendToClient(client,
                     ERR_NOSUCHCHANNEL(client->getNickname(), channelName) + "\r\n");
        return;
    }

    Channel *channel = getChannel(channelName);

    // Create channel
    if (!channel)
    {
        channel = new Channel(channelName);
        channels[channelName] = channel;

        channel->addMember(client);
        channel->addOperator(client);

        sendToClient(client,
                     ":" + client->getNickname() +
                         " JOIN " + channelName + "\r\n");

        if (channel->getTopic().empty())
        {
            sendToClient(client,
                         RPL_NOTOPIC(client->getNickname(),
                                     channelName) +
                             "\r\n");
        }
        else
        {
            sendToClient(client,
                         RPL_TOPIC(client->getNickname(),
                                   channelName,
                                   channel->getTopic()) +
                             "\r\n");
        }

        return;
    }

    if (channel->isMember(client))
    {
        return;
    }

    // MODE +i
    if (channel->isInviteOnly())
    {
        if (!channel->isInvited(client))
        {
            sendToClient(client,
                         ERR_INVITEONLYCHAN(client->getNickname(),
                                            channelName) +
                             "\r\n");
            return;
        }

        channel->removeInvitation(client);
    }

    // MODE +k
    if (channel->hasKey())
    {
        if (params.size() < 2 ||
            params[1] != channel->getKey())
        {
            sendToClient(client,
                         ERR_BADCHANNELKEY(client->getNickname(),
                                           channelName) +
                             "\r\n");
            return;
        }
    }

    // MODE +l
    if (channel->getUserLimit() > 0 &&
        channel->memberCount() >=
            static_cast<size_t>(channel->getUserLimit()))
    {
        sendToClient(client,
                     ERR_CHANNELISFULL(client->getNickname(),
                                       channelName) +
                         "\r\n");
        return;
    }

    channel->addMember(client);

    sendToClient(client,
                 ":" + client->getNickname() +
                     " JOIN " + channelName + "\r\n");

    if (channel->getTopic().empty())
    {
        sendToClient(client,
                     RPL_NOTOPIC(client->getNickname(),
                                 channelName) +
                         "\r\n");
    }
    else
    {
        sendToClient(client,
                     RPL_TOPIC(client->getNickname(),
                               channelName,
                               channel->getTopic()) +
                         "\r\n");
    }
}

void Server::partCommand(Client *client,
                         const std::vector<std::string> &params)
{
    if (!client->isRegistered())
    {
        sendToClient(client,
                     ERR_NOTREGISTERED(client->getNickname()) + "\r\n");
        return;
    }

    if (params.size() != 1)
    {
        sendToClient(client,
                     ERR_NEEDMOREPARAMS(client->getNickname(), "PART") + "\r\n");
        return;
    }

    std::string channelName = params[0];

    if (channelName.empty() || channelName[0] != '#')
    {
        sendToClient(client,
                     ERR_NOSUCHCHANNEL(client->getNickname(),
                                       channelName) +
                         "\r\n");
        return;
    }

    Channel *channel = getChannel(channelName);

    if (!channel)
    {
        sendToClient(client,
                     ERR_NOSUCHCHANNEL(client->getNickname(),
                                       channelName) +
                         "\r\n");
        return;
    }

    if (!channel->isMember(client))
    {
        sendToClient(client,
                     ERR_NOTONCHANNEL(client->getNickname(),
                                      channelName) +
                         "\r\n");
        return;
    }

    sendToClient(client,
                 ":" + client->getNickname() + " PART " + channelName + "\r\n");

    channel->removeMember(client);

    if (channel->memberCount() == 0)
    {
        channels.erase(channelName);
        delete channel;
    }
}