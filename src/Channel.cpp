#include "../include/Channel.hpp"



Channel::Channel() :
    name(""),
    topic("")
{
}

Channel::Channel(const std::string& name) :
    name(name),
    topic("")
{
}

Channel::~Channel()
{
}

const std::string& Channel::getName() const
{
    return name;
}

const std::string& Channel::getTopic() const
{
    return topic;
}

void Channel::setTopic(const std::string& topic)
{
    this->topic = topic;
}


