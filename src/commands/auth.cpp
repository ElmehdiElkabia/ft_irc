#include "../include/Server.hpp"

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

void Server::passCommand(Client *client, const std::vector<std::string> &params)
{
    if (params.size() != 1)
    {
        std::cerr << "Invalid number of parameters for PASS command." << std::endl;
        return;
    }
    if (client->hasPassed())
    {
        std::cerr << "You have already entered the password." << std::endl;
        return;
    }
    if (client->isRegistered())
    {
        std::cerr << "You are already registered." << std::endl;
        return;
    }
    std::string password = params[0];
    if (password != this->password)
    {
        std::cerr << "Incorrect password." << std::endl;
        return;
    }
    client->setPassAccepted(true);
}

void Server::nickCommand(Client *client, const std::vector<std::string> &params)
{
    if (params.size() != 1)
    {
        std::cerr << "Invalid number of parameters for NICK command." << std::endl;
        return;
    }
    if (!client->hasPassed())
    {
        std::cerr << "You must enter the password first." << std::endl;
        return;
    }
    std::string nickname = params[0];
    if (!isValidNickname(nickname))
    {
        std::cerr << "Invalid nickname." << std::endl;
        return;
    }
    if (getClientByNick(nickname) != nullptr)
    {
        std::cerr << "Nickname is already in use." << std::endl;
        return;
    }
    client->setNickname(nickname);
}

