#include "../../include/Server.hpp"


void Server::topicCommand(Client *client, const std::vector<std::string> &params)
{
    if (!client->isRegistered())
    {
        std::cerr << "You must be registered to set a topic." << std::endl;
        return;
    }
    if (params.size() < 1)
    {
        std::cerr << "Invalid number of parameters for TOPIC command." << std::endl;
        return;
    }
    std::string channelName = params[0];
    if (channelName[0] != '#')
    {
        std::cerr << "Invalid channel name. Channel names must start with '#'." << std::endl;
        return;
    }
    Channel *channel = getChannel(channelName);
    if (!channel)
    {
        std::cerr << "No such channel: " << channelName << std::endl;
        return;
    }
    if (!channel->isMember(client))
    {
        std::cerr << "You are not a member of channel: " << channelName << std::endl;
        return;
    }
    if (params.size() == 1)
    {
        std::cout << "Current topic for channel " << channelName << ": " << channel->getTopic() << std::endl;
        return;
    }
    std::string newTopic = params[1];
    channel->setTopic(newTopic);
    std::cout << "Topic for channel " << channelName << " set to: " << newTopic << std::endl;
}