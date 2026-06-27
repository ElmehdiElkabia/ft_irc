#include "../../include/Server.hpp"

bool isSpecial(char c)
{
    return (c == '-' || c == '[' || c == ']' || c == '\\' || c == '`' ||
            c == '^' || c == '{' || c == '}' || c == '|');
}

static bool isValidNickname(const std::string &nick)
{
    if (nick.empty())
        return false;
    if (!isalpha(nick[0]) && !isSpecial(nick[0]))
        return false;
    for (size_t i = 1; i < nick.length(); ++i)
    {
        if (!isalnum(nick[i]) && !isSpecial(nick[i]) && nick[i] != '-')
            return false;
    }
    return true;
}

static void checkRegistration(Client *client)
{
    if (client->isRegistered())
        return;

    if (client->hasPassed() && !client->getNickname().empty() && !client->getUsername().empty())
    {
        client->setRegistered(true);

        std::string welcomeMsg = RPL_WELCOME(client->getNickname(), client->getPrefix()) + "\r\n";

        send(client->getFd(), welcomeMsg.c_str(), welcomeMsg.size(), 0);
    }
}

#include <sys/socket.h>

void Server::passCommand(Client *client, const std::vector<std::string> &params)
{
    if (params.size() != 1)
    {
        sendToClient(client, ERR_NEEDMOREPARAMS(client->getNickname(), "PASS") + "\r\n");
        return;
    }

    if (client->hasPassed())
    {
        sendToClient(client, ERR_PASSWDMISMATCH(client->getNickname()) + "\r\n");
        return;
    }

    if (client->isRegistered())
    {
        sendToClient(client, ERR_ALREADYREGISTRED(client->getNickname()) + "\r\n");
        return;
    }

    if (params[0] != this->password)
    {
        sendToClient(client, ERR_PASSWDMISMATCH(client->getNickname()) + "\r\n");
        return;
    }

    client->setPassAccepted(true);

    checkRegistration(client);
}

void Server::nickCommand(Client *client, const std::vector<std::string> &params)
{
    if (params.size() != 1)
    {
        sendToClient(client, ERR_NEEDMOREPARAMS(client->getNickname(), "NICK") + "\r\n");
        return;
    }

    if (!client->hasPassed())
    {
        sendToClient(client, ERR_PASSWDMISMATCH(client->getNickname()) + "\r\n");
        return;
    }

    std::string nickname = params[0];

    if (!isValidNickname(nickname))
    {
        sendToClient(client, ERR_ERRONEUSNICKNAME(client->getNickname(), nickname) + "\r\n");
        return;
    }

    Client *existing = getClientByNick(nickname);

    if (existing && existing != client)
    {
        sendToClient(client, ERR_NICKNAMEINUSE(client->getNickname(), nickname) + "\r\n");
        return;
    }

    std::string oldNickname = client->getNickname();

    client->setNickname(nickname);
    if (client->isRegistered())
        sendToClient(client, ":" + oldNickname + " NICK :" + nickname + "\r\n");
    else
        checkRegistration(client);
}

void Server::userCommand(Client *client, const std::vector<std::string> &params)
{
    if (params.size() != 4)
    {
        sendToClient(client, ERR_NEEDMOREPARAMS(client->getNickname(), "USER") + "\r\n");
        return;
    }

    if (!client->hasPassed())
    {
        sendToClient(client, ERR_PASSWDMISMATCH(client->getNickname()) + "\r\n");
        return;
    }

    if (!client->getUsername().empty())
    {
        sendToClient(client, ERR_ALREADYREGISTRED(client->getNickname()) + "\r\n");
        return;
    }

    std::string username = params[0];

    if (username.empty())
    {
        sendToClient(client, ERR_NEEDMOREPARAMS(client->getNickname(), "USER") + "\r\n");
        return;
    }

    client->setUsername(username);
    client->setHostname(params[1]);
    client->setRealname(params[3]);

    checkRegistration(client);
}