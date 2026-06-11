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

void Server::inviteCommand(Client *client, const std::vector<std::string> &params)
{
    if (!client->isRegistered())
    {
        std::cerr << "You must be registered to invite someone to a channel." << std::endl;
        return;
    }
    if (params.size() != 2)
    {
        std::cerr << "Invalid number of parameters for INVITE command." << std::endl;
        return;
    }
    std::string nickname = params[0];
    Client *invitee = getClientByNick(nickname);
    if (!invitee)
    {
        std::cerr << "No such user: " << nickname << std::endl;
        return;
    }
    std::string channelName = params[1];
    if (channelName[0] != '#' || channelName.empty())
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
    if (channel->isMember(invitee))
    {
        std::cerr << "User " << nickname << " is already a member of channel:" << channelName << std::endl;
        return;
    }
    if (channel->isInvited(invitee))
    {
        std::cerr << "User " << nickname << " has already been invited to channel: " << channelName << std::endl;
        return;
    }
    channel->addInvited(invitee);
    std::cout << "User " << nickname << " has been invited to channel " << channelName << "." << std::endl;
}

void Server::kickCommand(Client *client, const std::vector<std::string> &params)
{
    if (!client->isRegistered())
    {
        std::cerr << "You must be registered to kick someone from a channel." << std::endl;
        return;
    }
    if (params.size() != 2)
    {
        std::cerr << "Invalid number of parameters for KICK command." << std::endl;
        return;
    }

    std::string channelName = params[0];
    if (channelName[0] != '#' || channelName.empty())
    {
        std::cerr << "Invalid channel name. Channel names must start with '#'." << std ::endl;
        return;
    }
    Channel *channel = getChannel(channelName);
    if (!channel)
    {
        std::cerr << "No such channel: " << channelName << std::endl;
        return;
    }
    std::string nickname = params[1];
    Client *kickee = getClientByNick(nickname);
    if (!kickee)
    {
        std::cerr << "No such user: " << nickname << std::endl;
        return;
    }
    if (!channel->isMember(client))
    {
        std::cerr << "You are not a member of channel: " << channelName << std::endl;
        return;
    }
    if (!channel->isOperator(client))
    {
        std::cerr << "You are not an operator of channel: " << channelName << std::endl;
        return;
    }
    if (!channel->isMember(kickee))
    {
        std::cerr << "User " << nickname << " is not a member of channel: " << channelName << std::endl;
        return;
    }
    channel->removeMember(kickee);
    std::cout << "User " << nickname << " has been kicked from channel " << channelName << "." << std::endl;
}