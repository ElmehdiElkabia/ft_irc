#include "../../include/Server.hpp"

void Server::topicCommand(Client *client, const std::vector<std::string> &params)
{
    if (!client->isRegistered())
    {
        sendToClient(client, ERR_NOTREGISTERED(client->getNickname()) + "\r\n");
        return;
    }
    if (params.size() < 1)
    {
        sendToClient(client, ERR_NEEDMOREPARAMS(client->getNickname(), "TOPIC") + "\r\n");
        return;
    }
    if (params.size() > 2)
    {
        sendToClient(client, ERR_NEEDMOREPARAMS(client->getNickname(), "TOPIC") + "\r\n");
        return;
    }
    std::string channelName = params[0];
    if (channelName[0] != '#')
    {
        sendToClient(client, ERR_NOSUCHCHANNEL(client->getNickname(), channelName) + "\r\n");
        return;
    }
    Channel *channel = getChannel(channelName);
    if (!channel)
    {
        sendToClient(client, ERR_NOSUCHCHANNEL(client->getNickname(), channelName) + "\r\n");
        return;
    }
    if (!channel->isMember(client))
    {
        sendToClient(client, ERR_NOTONCHANNEL(client->getNickname(), channelName) + "\r\n");
        return;
    }
    if (params.size() == 1)
    {
        std::string topic = channel->getTopic();
        if (topic.empty())
        {
            sendToClient(client, RPL_NOTOPIC(client->getNickname(), channelName) + "\r\n");
        }
        else
        {
            sendToClient(client, RPL_TOPIC(client->getNickname(), channelName, topic) + "\r\n");
        }
        return;
    }
    if (channel->isTopicRestricted() && !channel->isOperator(client))
    {
        sendToClient(client, ERR_CHANOPRIVSNEEDED(client->getNickname(), channelName) + "\r\n");
        return;
    }
    std::string newTopic = params[1];
    channel->setTopic(newTopic);
    for (size_t i = 0; i < channel->memberCount(); i++)
    {
        Client *member = channel->getMembers()[i];

        sendToClient(member, ":" + client->getNickname() + " TOPIC " + channelName + " :" + newTopic + "\r\n");
    }
}

void Server::inviteCommand(Client *client, const std::vector<std::string> &params)
{
    if (!client->isRegistered())
    {
        sendToClient(client, ERR_NOTREGISTERED(client->getNickname()) + "\r\n");
        return;
    }
    if (params.size() != 2)
    {
        sendToClient(client, ERR_NEEDMOREPARAMS(client->getNickname(), "INVITE") + "\r\n");
        return;
    }
    std::string nickname = params[0];
    Client *invitee = getClientByNick(nickname);
    if (!invitee)
    {
        sendToClient(client, ERR_NOSUCHNICK(client->getNickname(), nickname) + "\r\n");
        return;
    }
    std::string channelName = params[1];
    if (channelName[0] != '#' || channelName.empty())
    {
        sendToClient(client, ERR_NOSUCHCHANNEL(client->getNickname(), channelName) + "\r\n");
        return;
    }
    Channel *channel = getChannel(channelName);
    if (!channel)
    {
        sendToClient(client, ERR_NOSUCHCHANNEL(client->getNickname(), channelName) + "\r\n");
        return;
    }
    if (!channel->isMember(client))
    {
        sendToClient(client, ERR_NOTONCHANNEL(client->getNickname(), channelName) + "\r\n");
        return;
    }
    if (channel->isMember(invitee))
    {
        sendToClient(client, ERR_USERONCHANNEL(client->getNickname(), nickname, channelName) + "\r\n");
        return;
    }
    if (channel->isInvited(invitee))
    {
        sendToClient(client, ERR_USERONCHANNEL(client->getNickname(), nickname, channelName) + "\r\n");
        return;
    }
    channel->addInvited(invitee);
    sendToClient(client, RPL_INVITING(client->getNickname(), channelName, nickname) + "\r\n");
    sendToClient(invitee, ":" + client->getNickname() + " INVITE " + nickname + " :" + channelName + "\r\n");
}

void Server::kickCommand(Client *client, const std::vector<std::string> &params)
{
    if (!client->isRegistered())
    {
        sendToClient(client, ERR_NOTREGISTERED(client->getNickname()) + "\r\n");
        return;
    }
    if (params.size() < 2)
    {
        sendToClient(client, ERR_NEEDMOREPARAMS(client->getNickname(), "KICK") + "\r\n");
        return;
    }

    std::string channelName = params[0];
    if (channelName[0] != '#' || channelName.empty())
    {
        sendToClient(client, ERR_NOSUCHCHANNEL(client->getNickname(), channelName) + "\r\n");
        return;
    }
    Channel *channel = getChannel(channelName);
    if (!channel)
    {
        sendToClient(client, ERR_NOSUCHCHANNEL(client->getNickname(), channelName) + "\r\n");
        return;
    }
    std::string nickname = params[1];
    Client *kickee = getClientByNick(nickname);
    if (!kickee)
    {
        sendToClient(client, ERR_NOSUCHNICK(client->getNickname(), nickname) + "\r\n");
        return;
    }
    if (!channel->isMember(client))
    {
        sendToClient(client, ERR_NOTONCHANNEL(client->getNickname(), channelName) + "\r\n");
        return;
    }
    if (!channel->isOperator(client))
    {
        sendToClient(client, ERR_CHANOPRIVSNEEDED(client->getNickname(), channelName) + "\r\n");
        return;
    }
    if (!channel->isMember(kickee))
    {
        sendToClient(client, ERR_USERNOTINCHANNEL(client->getNickname(), nickname, channelName) + "\r\n");
        return;
    }
    std::string msg = ":" + client->getNickname() + " KICK " + channelName + " " + nickname + "\r\n";

    for (size_t i = 0; i < channel->memberCount(); i++)
    {
        sendToClient(channel->getMembers()[i], msg);
    }

    sendToClient(kickee, msg);
    channel->removeMember(kickee);
}