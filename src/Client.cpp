#include "../include/Client.hpp"


Client::Client() :
    fd(-1),
    passAccepted(false),
    registered(false)
{
}

Client::Client(int fd) :
    fd(fd),
    passAccepted(false),
    registered(false)
{
}

Client::~Client()
{
}

int Client::getFd() const
{
    return fd;
}

const std::string& Client::getNickname() const
{
    return nickname;
}

const std::string& Client::getUsername() const
{
    return username;
}

void Client::setNickname(const std::string& nick)
{
    nickname = nick;
}

void Client::setUsername(const std::string& user)
{
    username = user;
}

void Client::setRealname(const std::string& real)
{
    realname = real;
}

bool Client::isRegistered() const
{
    return registered;
}

bool Client::hasPassed() const
{
    return passAccepted;
}

void Client::setPassAccepted(bool value)
{
    passAccepted = value;
}

void Client::setRegistered(bool value)
{
    registered = value;
}