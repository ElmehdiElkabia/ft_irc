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
            server->sendToClient(client, ":" + client->getNickname() + " MODE " + channel->getName() + " +i\r\n");
        }
        else if (sign == '-')
        {
            channel->setInviteOnly(false);
            server->sendToClient(client, ":" + client->getNickname() + " MODE " + channel->getName() + " -i\r\n");
        }
    }
    else if (modeChar == 't')
    {
        if (sign == '+')
        {
            channel->setTopicRestricted(true);
            server->sendToClient(client, ":" + client->getNickname() + " MODE " + channel->getName() + " +t\r\n");
        }
        else if (sign == '-')
        {
            channel->setTopicRestricted(false);
            server->sendToClient(client, ":" + client->getNickname() + " MODE " + channel->getName() + " -t\r\n");
        }
    }
    else if (modeChar == 'k')
    {
        if (sign == '+')
        {
            if (params.size() != 3)
            {
                server->sendToClient(client, ERR_NEEDMOREPARAMS(client->getNickname(), "MODE") + "\r\n");
                return;
            }

            channel->setKey(params[2]);

            server->sendToClient(client, ":" + client->getNickname() + " MODE " + channel->getName() + " +k " + params[2] + "\r\n");
        }
        else if (sign == '-')
        {
            channel->setKey("");

            server->sendToClient(client, ":" + client->getNickname() + " MODE " + channel->getName() + " -k\r\n");
        }
    }
    else if (modeChar == 'l')
    {
        if (sign == '+')
        {
            if (params.size() != 3)
            {
                server->sendToClient(client, ERR_NEEDMOREPARAMS(client->getNickname(), "MODE") + "\r\n");
                return;
            }

            int limit = std::atoi(params[2].c_str());
            channel->setUserLimit(limit);

            server->sendToClient(client, ":" + client->getNickname() + " MODE " + channel->getName() + " +l " + params[2] + "\r\n");
        }
        else if (sign == '-')
        {
            channel->setUserLimit(0);

            server->sendToClient(client, ":" + client->getNickname() + " MODE " + channel->getName() + " -l\r\n");
        }
    }
    else if (modeChar == 'o')
    {
        if (sign == '+')
        {
            if (params.size() != 3)
            {
                server->sendToClient(client, ERR_NEEDMOREPARAMS(client->getNickname(), "MODE") + "\r\n");
                return;
            }

            Client *target = server->getClientByNick(params[2]);

            if (!target)
            {
                server->sendToClient(client, ERR_NOSUCHNICK(client->getNickname(), params[2]) + "\r\n");
                return;
            }

            if (!channel->isMember(target))
            {
                server->sendToClient(client, ERR_USERNOTINCHANNEL(client->getNickname(), params[2], channel->getName()) + "\r\n");
                return;
            }

            channel->addOperator(target);

            std::string msg = ":" + client->getNickname() + " MODE " + channel->getName() + " +o " + target->getNickname() + "\r\n";

            for (size_t i = 0; i < channel->memberCount(); i++)
            {
                server->sendToClient(channel->getMembers()[i], msg);
            }
        }
        else if (sign == '-')
        {
            if (params.size() != 3)
            {
                server->sendToClient(client, ERR_NEEDMOREPARAMS(client->getNickname(), "MODE") + "\r\n");
                return;
            }

            Client *target = server->getClientByNick(params[2]);

            if (!target)
            {
                server->sendToClient(client, ERR_NOSUCHNICK(client->getNickname(), params[2]) + "\r\n");
                return;
            }

            if (!channel->isMember(target))
            {
                server->sendToClient(client, ERR_USERNOTINCHANNEL(client->getNickname(), params[2], channel->getName()) + "\r\n");
                return;
            }

            channel->removeOperator(target);

            std::string msg = ":" + client->getNickname() + " MODE " + channel->getName() + " -o " + target->getNickname() + "\r\n";

            for (size_t i = 0; i < channel->memberCount(); i++)
            {
                server->sendToClient(channel->getMembers()[i], msg);
            }
        }
    }
    else
    {
        server->sendToClient(client, ERR_UNKNOWNMODE(client->getNickname(), mode) + "\r\n");
    }
}

void Server::modeCommand(Client *client, const std::vector<std::string> &params)
{
    if (!client->isRegistered())
    {
        sendToClient(client, ERR_NOTREGISTERED(client->getNickname()) + "\r\n");
        return;
    }
    if (params.size() < 2)
    {
        sendToClient(client, ERR_NEEDMOREPARAMS(client->getNickname(), "MODE") + "\r\n");
        return;
    }
    std::string target = params[0];
    if (target[0] != '#')
    {
        sendToClient(client, ERR_NOSUCHCHANNEL(client->getNickname(), target) + "\r\n");
        return;
    }

    Channel *channel = getChannel(target);
    if (!channel)
    {
        sendToClient(client, ERR_NOSUCHCHANNEL(client->getNickname(), target) + "\r\n");
        return;
    }
    if (!channel->isMember(client))
    {
        sendToClient(client, ERR_NOTONCHANNEL(client->getNickname(), client->getNickname()) + "\r\n");
        return;
    }
    if (!channel->isOperator(client))
    {
        sendToClient(client, ERR_CHANOPRIVSNEEDED(client->getNickname(), target) + "\r\n");
        return;
    }
    std::string mode = params[1];
    if (mode[0] != '+' && mode[0] != '-')
    {
        sendToClient(client, ERR_UNKNOWNMODE(client->getNickname(), mode) + "\r\n");
        return;
    }
    handleModeChange(this, client, channel, mode, params);
}