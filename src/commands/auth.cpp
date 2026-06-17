#include "../../include/Server.hpp"

bool isSpecial(char c)
{
    return (c == '-' || c == '[' || c == ']' || c == '\\' || c == '`' ||
            c == '^' || c == '{' || c == '}' || c == '|');
}

static bool isValidNickname(const std::string &nick)
{
    if (nick.empty() || nick.length() > 9)
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
    if (client->hasPassed() && !client->getNickname().empty() && !client->getUsername().empty())
    {
        client->setRegistered(true);
        std::string welcomeMsg = "Welcome to the IRC server, " + client->getNickname() + "!\r\n";
        send(client->getFd(), welcomeMsg.c_str(), welcomeMsg.size(), 0);}
}

#include <sys/socket.h>

void Server::passCommand(Client *client, const std::vector<std::string> &params)
{
    if (params.size() != 1)
    {
        sendToClient(client, "ERROR :Invalid number of parameters for PASS command\r\n");
        return;
    }

    if (client->hasPassed())
    {
        sendToClient(client, "ERROR :Password already accepted\r\n");
        return;
    }

    if (client->isRegistered())
    {
        sendToClient(client, "ERROR :You are already registered\r\n");
        return;
    }

    if (params[0] != this->password)
    {
        sendToClient(client, "ERROR :Incorrect password\r\n");
        return;
    }

    client->setPassAccepted(true);

    sendToClient(client, ":ircserv NOTICE * :Password accepted\r\n");

    checkRegistration(client);
}

void Server::nickCommand(Client *client, const std::vector<std::string> &params)
{
    if (params.size() != 1)
    {
        sendToClient(client, "ERROR :Invalid number of parameters for NICK command\r\n");
        return;
    }

    if (!client->hasPassed())
    {
        sendToClient(client, "ERROR :You must enter the password first\r\n");
        return;
    }

    std::string nickname = params[0];

    if (!isValidNickname(nickname))
    {
        sendToClient(client, "ERROR :Invalid nickname\r\n");
        return;
    }

    Client *existing = getClientByNick(nickname);

    if (existing && existing != client)
    {
        sendToClient(client, "ERROR :Nickname is already in use\r\n");
        return;
    }

    // if (getClientByNick(nickname) != NULL)
    // {
    //     sendToClient(client, "ERROR :Nickname is already in use\r\n");
    //     return;
    // }

    client->setNickname(nickname);
    sendToClient(client, ":ircserv NOTICE * :Nickname accepted\r\n");
    checkRegistration(client);
}

void Server::userCommand(Client *client, const std::vector<std::string> &params)
{
    if (params.size() != 4)
    {
        sendToClient(client, "ERROR :Invalid number of parameters for USER command\r\n");
        return;
    }
    if (!client->hasPassed())
    {
        sendToClient(client, "ERROR :You must enter the password first\r\n");
        return;
    }
    if (!client->getUsername().empty())
    {
        sendToClient(client, "ERROR :You must set a nickname first\r\n");
        return;
    }
    std::string username = params[0];
    if (username.empty())
    {
        sendToClient(client, "ERROR :Username cannot be empty\r\n");
        return;
    }
    client->setUsername(username);
    client->setRealname(params[3]);
    client->setRegistered(true);
    sendToClient(client, ":ircserv NOTICE * :User registration successful\r\n");
    checkRegistration(client);
}
