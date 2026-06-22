#include "../../include/Server.hpp"

static void handleModeChange(Server *server, Client *client, Channel *channel, const std::string &mode, const std::vector<std::string> &params)
{
    char sign = mode[0];
    char modeChar = mode[1];
    if (modeChar == 'i')
    {
        if (sign == '+')
        {
            channel->setInviteOnly(true);
            server->sendToClient(client, "Channel " + channel->getName() + " is now invite-only.\r\n");
        }
        else if (sign == '-')
        {
            channel->setInviteOnly(false);
            server->sendToClient(client, "Channel " + channel->getName() + " is no longer invite-only.\r\n");
        }
    }
    else if (modeChar == 't')
    {
        if (sign == '+')
        {
            channel->setTopicRestricted(true);
            server->sendToClient(client, "Channel " + channel->getName() + " is now topic-restricted.\r\n");
        }
        else if (sign == '-')
        {
            channel->setTopicRestricted(false);
            server->sendToClient(client, "Channel " + channel->getName() + " is no longer topic-restricted.\r\n");
        }
    }
    else if (modeChar == 'k')
    {
        if (sign == '+')
        {
            if (params.size() != 3)
            {
                server->sendToClient(client, "ERROR :Missing password for channel " + channel->getName() + ".\r\n");
                return;
            }

            channel->setKey(params[2]);

            server->sendToClient(client, "Password set for channel " + channel->getName() + ".\r\n");
        }
        else if (sign == '-')
        {
            channel->setKey("");

            server->sendToClient(client, "Password removed for channel " + channel->getName() + ".\r\n");
        }
    }
    else if (modeChar == 'l')
    {
        if (sign == '+')
        {
            if (params.size() != 3)
            {
                server->sendToClient(client, "ERROR :Missing user limit for channel " + channel->getName() + ".\r\n");
            }

            int limit = std::atoi(params[2].c_str());
            channel->setUserLimit(limit);

            server->sendToClient(client, "User limit for channel " + channel->getName() + ".\r\n");
        }
        else if (sign == '-')
        {
            channel->setUserLimit(0);

            server->sendToClient(client, "User limit removed for channel " + channel->getName() + ".\r\n");
        }
    }
    else if (modeChar == 'o')
    {
        if (sign == '+')
        {
            if (params.size() != 3)
            {
                server->sendToClient(client, "ERROR :Missing nickname for operator mode change.\r\n");
            }

            Client *target = server->getClientByNick(params[2]);

            if (!target)
            {
                server->sendToClient(client, "ERROR :No such user: " + params[2] + "\r\n");
                return;
            }

            if (!channel->isMember(target))
            {
                server->sendToClient(client, "ERROR :User " + params[2] + " is not a member of channel: " + channel->getName() + "\r\n");
                return;
            }

            channel->addOperator(target);

            server->sendToClient(client, "User " + params[2] + " is now an operator of channel " + channel->getName() + ".\r\n");
        }
        else if (sign == '-')
        {
            if (params.size() != 3)
            {
                server->sendToClient(client, "ERROR :Missing nickname for operator mode change.\r\n");
                return;
            }

            Client *target = server->getClientByNick(params[2]);

            if (!target)
            {
                server->sendToClient(client, "ERROR :No such user: " + params[2] + "\r\n");
                return;
            }

            if (!channel->isMember(target))
            {
                server->sendToClient(client, "ERROR :User " + params[2] + " is not a member of channel: " + channel->getName() + "\r\n");
                return;
            }

            channel->removeOperator(target);

            server->sendToClient(client, "User " + params[2] + " is no longer an operator of channel " + channel->getName() + ".\r\n");
        }
    }
}

void Server::modeCommand(Client *client, const std::vector<std::string> &params)
{
    if (!client->isRegistered())
    {
        sendToClient(client, "ERROR :You must be registered to use the MODE command\r\n");
        return;
    }
    if (params.size() < 2)
    {
        sendToClient(client, "ERROR :Invalid number of parameters for MODE command\r\n");
        return;
    }
    std::string target = params[0];
    if (target[0] != '#')
    {
        sendToClient(client, "ERROR :Invalid channel name. Channel names must start with '#'.\r\n");
        return;
    }

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
    if (!channel->isOperator(client))
    {
        sendToClient(client, "ERROR :You must be an operator to change modes for channel: " + target + "\r\n");
        return;
    }
    std::string mode = params[1];
    if (mode[0] != '+' && mode[0] != '-')
    {
        sendToClient(client, "ERROR :Invalid mode format. Mode must start with '+' or '-'.\r\n");
        return;
    }
    handleModeChange(this, client, channel, mode, params);
}