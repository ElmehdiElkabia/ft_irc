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
        sendToClient(client, "TOPIC " + channelName + " :" + channel->getTopic() + "\r\n");
        return;
    }
    std::string newTopic = params[1];
    channel->setTopic(newTopic);
    sendToClient(client, "TOPIC " + channelName + " :" + newTopic + "\r\n");
}