#include "../include/Channel.hpp"

Channel::Channel() : name(""),
                     topic("")
{
    inviteOnly = false;
    topicRestricted = false;
    password = "";
    userLimit = 0;
}

Channel::Channel(const std::string &name) : name(name),
                                            topic("")
{
    inviteOnly = false;
    topicRestricted = false;
    password = "";
    userLimit = 0;
}

Channel::~Channel()
{
}

const std::string &Channel::getName() const
{
    return name;
}

const std::string &Channel::getTopic() const
{
    return topic;
}

void Channel::setTopic(const std::string &topic)
{
    this->topic = topic;
}

void Channel::addMember(Client *client)
{
    members.push_back(client);
}

void Channel::removeMember(Client *client)
{
    for (std::vector<Client *>::iterator it = members.begin(); it != members.end(); ++it)
    {
        if (*it == client)
        {
            members.erase(it);
            break;
        }
    }
}

bool Channel::isMember(Client *client) const
{
    for (std::vector<Client *>::const_iterator it = members.begin(); it != members.end(); ++it)
    {
        if (*it == client)
            return true;
    }
    return false;
}

bool Channel::isOperator(Client *client) const
{
    for (std::vector<Client *>::const_iterator it = operators.begin(); it != operators.end(); ++it)
    {
        if (*it == client)
            return true;
    }
    return false;
}

size_t Channel::memberCount() const
{
    return members.size();
}


std::vector<Client *> &Channel::getMembers()
{
    return members;
}

const std::vector<Client *> &Channel::getMembers() const
{
    return members;
}

void Channel::addInvited(Client *client)
{
    invitedClients.push_back(client);
}

bool Channel::isInvited(Client *client) const
{
    for (std::vector<Client *>::const_iterator it = invitedClients.begin(); it != invitedClients.end(); ++it)
    {
        if (*it == client)
            return true;
    }
    return false;
}

void Channel::removeInvitation(Client *client)
{
    for (std::vector<Client *>::iterator it = invitedClients.begin(); it != invitedClients.end(); ++it)
    {
        if (*it == client)
        {
            invitedClients.erase(it);
            break;
        }
    }
}

void Channel::addOperator(Client *client)
{
    if (!isOperator(client))
        operators.push_back(client);
}