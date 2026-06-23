#ifndef REPLIES_HPP
#define REPLIES_HPP

#define RPL_WELCOME(nick, user, host) "001 " + nick + " :Welcome to the Internet Relay Network " + nick + "!" + user + "@" + host

#define RPL_NOTOPIC(nick, channel, topic) "331 " + nick + " " + channel + " :No topic is set"

#define RPL_TOPIC(nick, channel, topic) "332 " + nick + " " + channel + " :" + topic

#define RPL_INVITING(nick, channel, invited) "341 " + nick + " " + channel + " " + invited

#define RPL_NOSUCHNICK(nick, target) "401 " + nick + " " + target + " :No such nick/channel"

#define RPL_NOSUCHCHANNEL(nick, channel) "403 " + nick + " " + channel + " :No such channel"

#define ERR_NORECIPIENT(nick, command) "411 " + nick + " :No recipient given (" + command + ")"

#define ERR_NOTEXTTOSEND(nick) "412 " + nick + " :No text to send"

#define ERR_NONICKNAMEGIVEN(nick) "431 " + nick + " :No nickname given"

#define ERR_NICKNAMEINUSE(nick, target) "433 " + nick + " " + target + " :Nickname is already in use"

#define ERR_ERRONEUSNICKNAME(nick, target) "432 " + nick + " " + target + " :Erroneous nickname"

#define ERR_USERNOTINCHANNEL(nick, user, channel) "441 " + nick + " " + user + " " + channel + " :They aren't on that channel"

#define ERR_NOTONCHANNEL(nick, channel) "442 " + nick + " " + channel + " :You're not on that channel"

#define ERR_USERONCHANNEL(nick, user, channel) "443 " + nick + " " + user + " " + channel + " :is already on channel"

#define ERR_NOTREGISTERED(nick) "451 " + nick + " :You have not registered"

#define ERR_NEEDMOREPARAMS(nick, command) "461 " + nick + " " + command + " :Not enough parameters"

#define ERR_ALREADYREGISTRED(nick) "462 " + nick + " :You may not reregister"

#define ERR_PASSWDMISMATCH(nick) "464 " + nick + " :Password incorrect"

#define ERR_CHANNELISFULL(nick, channel) "471 " + nick + " " + channel + " :Cannot join channel (+l)"

#define ERR_INVITEONLYCHAN(nick, channel) "473 " + nick + " " + channel + " :Cannot join channel (+i)"

#define ERR_BADCHANNELKEY(nick, channel) "475 " + nick + " " + channel + " :Cannot join channel (+k)"

#define ERR_CHANOPRIVSNEEDED(nick, channel) "482 " + nick + " " + channel + " :You're not channel operator"

#endif