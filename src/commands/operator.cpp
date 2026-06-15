#include "../../include/Server.hpp"

void Server::topicCommand(Client *client, const std::vector<std::string> &params)
{
    if (!client->isRegistered())
    {
        sendToClient(client, "ERROR :You must be registered to set or view a topic\r\n");
        return;
    }
    if (params.size() < 1)
    {
        sendToClient(client, "ERROR :Invalid number of parameters for TOPIC command\r\n");
        return;
    }
    if (params.size() > 2)
    {
        sendToClient(client, "ERROR :Too many parameters for TOPIC command\r\n");
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
    if (params.size() == 1)
    {
        std::cout << "Current topic for channel " << channelName << ": " << channel->getTopic() << std::endl;
        return;
    }
    std::string newTopic = params[1];
    channel->setTopic(newTopic);
    sendToClient(client, "TOPIC " + channelName + " :" + newTopic + "\r\n");
}

void Server::inviteCommand(Client *client, const std::vector<std::string> &params)
{
    if (!client->isRegistered())
    {
        sendToClient(client, "ERROR :You must be registered to invite someone to a channel\r\n");
        return;
    }
    if (params.size() != 2)
    {
        sendToClient(client, "ERROR :Invalid number of parameters for INVITE command\r\n");
        return;
    }
    std::string nickname = params[0];
    Client *invitee = getClientByNick(nickname);
    if (!invitee)
    {
        sendToClient(client, "ERROR :No such user: " + nickname + "\r\n");
        return;
    }
    std::string channelName = params[1];
    if (channelName[0] != '#' || channelName.empty())
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
    if (channel->isMember(invitee))
    {
        sendToClient(client, "ERROR :User " + nickname + " is already a member of channel: " + channelName + "\r\n");
        return;
    }
    if (channel->isInvited(invitee))
    {
        sendToClient(client, "ERROR :User " + nickname + " has already been invited to channel: " + channelName + "\r\n");
        return;
    }
    channel->addInvited(invitee);
    sendToClient(client, "User " + nickname + " has been invited to channel " + channelName + "." + "\r\n");
}

void Server::kickCommand(Client *client, const std::vector<std::string> &params)
{
    if (!client->isRegistered())
    {
        sendToClient(client, "ERROR :You must be registered to kick someone from a channel\r\n");
        return;
    }
    if (params.size() != 2)
    {
        sendToClient(client, "ERROR :Invalid number of parameters for KICK command\r\n");
        return;
    }

    std::string channelName = params[0];
    if (channelName[0] != '#' || channelName.empty())
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
    std::string nickname = params[1];
    Client *kickee = getClientByNick(nickname);
    if (!kickee)
    {
        sendToClient(client, "ERROR :No such user: " + nickname + "\r\n");
        return;
    }
    if (!channel->isMember(client))
    {
        sendToClient(client, "ERROR :You are not a member of channel: " + channelName + "\r\n");
        return;
    }
    if (!channel->isOperator(client))
    {
        sendToClient(client, "ERROR :You must be an operator to kick someone from channel: " + channelName + "\r\n");
        return;
    }
    if (!channel->isMember(kickee))
    {
        sendToClient(client, "ERROR :User " + nickname + " is not a member of channel: " + channelName + "\r\n");
        return;
    }
    channel->removeMember(kickee);
    sendToClient(kickee, "You have been kicked from channel: " + channelName + "\r\n");
}