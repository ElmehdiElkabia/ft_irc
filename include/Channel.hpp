#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <vector>
#include "Client.hpp"
#include <iostream>

class Client;

class Channel
{
private:
    std::string name;
    std::string topic;
    std::vector<Client *> members;
    std::vector<Client *> operators;

    std::vector<Client *> invitedClients;

    bool inviteOnly;
    bool topicRestricted;

    std::string password;

    size_t userLimit;

public:
    Channel();
    Channel(const std::string &name);
    ~Channel();

    const std::string &getName() const;
    const std::string &getTopic() const;

    std::vector<Client *> &getMembers();
    const std::vector<Client *> &getMembers() const;

    void setTopic(const std::string &topic);

    void addMember(Client *client);
    void removeMember(Client *client);

    size_t memberCount() const;

    bool isMember(Client *client) const;
    bool isOperator(Client *client) const;
    void removeOperator(Client *client);
    void addOperator(Client *client);

    void addInvited(Client *client);
    bool isInvited(Client *client) const;
    void removeInvitation(Client *client);

    void setInviteOnly(bool value);
    bool isInviteOnly() const;

    bool isTopicRestricted() const;
    void setTopicRestricted(bool value);

    std::string getKey() const;
    void setKey(const std::string &key);

    bool hasKey() const;

    int getUserLimit() const;
    void setUserLimit(int limit);
};

#endif