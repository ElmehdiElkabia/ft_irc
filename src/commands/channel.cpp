#include "../../include/Server.hpp"

void Server::joinCommand(Client *client, const std::vector<std::string> &params)
{

    if (!client->isRegistered())
    {
        std::cerr << "You must be registered to join a channel." << std::endl;
        return;
    }
    if (params.size() != 1)
    {
        std::cerr << "Invalid number of parameters for JOIN command." << std::endl;
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
        channel = new Channel(channelName);
        channels[channelName] = channel;
    }
    if (channel->isMember(client))
    {
        std::cerr << "You are already a member of channel: " << channelName << std::endl;
        return;
    }
    channel->addMember(client);
    std::cout << "Client " << client->getNickname() << " joined channel " << channelName << "." << std::endl;

}

void Server::partCommand(Client *client, const std::vector<std::string> &params)
{
    if (!client->isRegistered())
    {
        std::cerr << "You must be registered to part a channel." << std::endl;
        return;
    }
    if (params.size() != 1)
    {
        std::cerr << "Invalid number of parameters for PART command." << std::endl;
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
    channel->removeMember(client);
    if (channel->memberCount() == 1)
    {
        delete channel;
        channels.erase(channelName);
        std::cout << "Channel " << channelName << " has been deleted as it has no more members." << std::endl;
        return;
    }
    std::cout << "Client " << client->getNickname() << " left channel " << channelName << "." << std::endl;
}

void Server::quitCommnand(Client *client, const std::vector<std::string> &params)
{

    (void)params;
    // you check if the client is registerd
    // you remove the client from all channels they are a member
    // you delete the client from the server's client list
    // you should close the socket of the client
    // okay a khay abdo
    // rah dart likk koulchii hena
    std::cout << "Client " << client->getNickname() << " is quitting." << std::endl;
}