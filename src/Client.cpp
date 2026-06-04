#include "../include/Client.hpp"


Client::Client() :
    _fd(-1),
    _passAccepted(false),
    _registered(false)
{
}

Client::Client(int fd) :
    _fd(fd),
    _passAccepted(false),
    _registered(false)
{
}

Client::~Client()
{
}

int Client::getFd() const
{
    return _fd;
}

const std::string& Client::getNickname() const
{
    return _nickname;
}

const std::string& Client::getUsername() const
{
    return _username;
}

void Client::setNickname(const std::string& nick)
{
    _nickname = nick;
}

void Client::setUsername(const std::string& user)
{
    _username = user;
}

bool Client::isRegistered() const
{
    return _registered;
}

bool Client::hasPassed() const
{
    return _passAccepted;
}

void Client::setPassAccepted(bool value)
{
    _passAccepted = value;
}

void Client::setRegistered(bool value)
{
    _registered = value;
}