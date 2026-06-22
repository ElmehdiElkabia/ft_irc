#include "../../include/Server.hpp"

static void handleModeChange(Client *client, Channel *channel, const std::string &mode, const std::vector<std::string> &params)
{
    char sign = mode[0];
    char modeChar = mode[1];
    if (modeChar == 'i')
    {
        if (sign == '+')
        {
            channel->setInviteOnly(true);
            std::cout << "Channel " << channel->getName() << " is now invite-only." << std::endl;
        }
        else if (sign == '-')
        {
            channel->setInviteOnly(false);
            std::cout << "Channel " << channel->getName() << " is no longer invite-only." << std::endl;
        }
    }
    else if (modeChar == 't')
    {
        if (sign == '+')
        {
            channel->setTopicRestricted(true);
            std::cout << "Channel " << channel->getName() << " is now topic-restricted." << std::endl;
        }
        else if (sign == '-')
        {
            channel->setTopicRestricted(false);
            std::cout << "Channel " << channel->getName() << " is no longer topic-restricted." << std::endl;
        }
    }
    else if (modeChar == 'k')
    {
        if (sign == '+')
        {
            if (params.size() != 3)
            {
                std::cerr << "Missing key." << std::endl;
                return;
            }

            channel->setKey(params[2]);

            std::cout
                << "Password set for channel "
                << channel->getName()
                << std::endl;
        }
        else if (sign == '-')
        {
            channel->setKey("");

            std::cout
                << "Password removed for channel "
                << channel->getName()
                << std::endl;
        }
    }
    else if (modeChar == 'l')
    {
        if (sign == '+')
        {
            if (params.size() != 3)
            {
                std::cerr << "Missing user limit." << std::endl;
                return;
            }

            int limit = std::stoi(params[2]);
            channel->setUserLimit(limit);

            std::cout
                << "User limit set to "
                << limit
                << " for channel "
                << channel->getName()
                << std::endl;
        }
        else if (sign == '-')
        {
            channel->setUserLimit(0);

            std::cout
                << "User limit removed for channel "
                << channel->getName()
                << std::endl;
        }
    }
    else if (modeChar == 'o')
    {
        if (sign == '+')
        {
            if (params.size() != 3)
            {
                std::cerr << "Missing nickname." << std::endl;
                return;
            }

            Client *target = getClientByNick(params[2]);

            if (!target)
            {
                std::cerr << "No such user." << std::endl;
                return;
            }

            if (!channel->isMember(target))
            {
                std::cerr << "User is not in the channel."
                          << std::endl;
                return;
            }

            channel->addOperator(target);

            std::cout
                << target->getNickname()
                << " is now an operator of "
                << channel->getName()
                << std::endl;
        }
        else if (sign == '-')
        {
            if (params.size() != 3)
            {
                std::cerr << "Missing nickname." << std::endl;
                return;
            }

            Client *target = getClientByNick(params[2]);

            if (!target)
            {
                std::cerr << "No such user." << std::endl;
                return;
            }

            if (!channel->isMember(target))
            {
                std::cerr << "User is not in the channel."
                          << std::endl;
                return;
            }

            channel->removeOperator(target);

            std::cout
                << target->getNickname()
                << " is no longer an operator of "
                << channel->getName()
                << std::endl;
        }
    }
}

void Server::modeCommand(Client *client, const std::vector<std::string> &params)
{
    if (!client->isRegistered())
    {
        std::cerr << "Client is not registered. Cannot set mode." << std::endl;
        return;
    }
    if (params.size() < 2)
    {
        std::cerr << "Not enough parameters for MODE command." << std::endl;
        return;
    }
    std::string target = params[0];
    if (target[0] != '#')
    {
        std::cerr << "Invalid target for MODE command. Target should be a channel starting with '#'." << std::endl;
        return;
    }

    Channel *channel = getChannel(target);
    if (!channel)
    {
        std::cerr << "Channel " << target << " does not exist." << std::endl;
        return;
    }
    if (!channel->isMember(client))
    {
        std::cerr << "Client is not a member of channel " << target << "." << std::endl;
        return;
    }
    if (!channel->isOperator(client))
    {
        std::cerr << "Client is not an operator of channel " << target << "." << std::endl;
        return;
    }
    std::string mode = params[1];
    if (mode[0] != '+' && mode[0] != '-')
    {
        std::cerr << "Invalid mode format. Mode should start with '+' or '-'." << std::endl;
        return;
    }
    handleModeChange(client, channel, mode, params);
}