#ifndef REPLIES_HPP
#define REPLIES_HPP

#define RPL_WELCOME(nick, prefix) ":ft_irc 001 " + nick + " :Welcome to the Internet Relay Network " + prefix

#define RPL_NOTOPIC(nick, channel) ":ft_irc 331 " + nick + " " + channel + " :No topic is set"

#define RPL_TOPIC(nick, channel, topic) ":ft_irc 332 " + nick + " " + channel + " :" + topic

#define RPL_INVITING(nick, channel, invited) ":ft_irc 341 " + nick + " " + channel + " " + invited

#define ERR_NOSUCHNICK(nick, target) ":ft_irc 401 " + nick + " " + target + " :No such nick/channel"

#define ERR_NOSUCHCHANNEL(nick, channel) ":ft_irc 403 " + nick + " " + channel + " :No such channel"

#define ERR_NORECIPIENT(nick, command) ":ft_irc 411 " + nick + " :No recipient given (" + command + ")"

#define ERR_NOTEXTTOSEND(nick) ":ft_irc 412 " + nick + " :No text to send"

#define ERR_CANNOTSENDTOCHAN(nick, channel) ":ft_irc 404 " + nick + " " + channel + " :Cannot send to channel"

#define ERR_NONICKNAMEGIVEN(nick) ":ft_irc 431 " + nick + " :No nickname given"

#define ERR_NICKNAMEINUSE(nick, target) ":ft_irc 433 " + nick + " " + target + " :Nickname is already in use"

#define ERR_ERRONEUSNICKNAME(nick, target) ":ft_irc 432 " + nick + " " + target + " :Erroneous nickname"

#define ERR_USERNOTINCHANNEL(nick, user, channel) ":ft_irc 441 " + nick + " " + user + " " + channel + " :They aren't on that channel"

#define ERR_NOTONCHANNEL(nick, channel) ":ft_irc 442 " + nick + " " + channel + " :You're not on that channel"

#define ERR_USERONCHANNEL(nick, user, channel) ":ft_irc 443 " + nick + " " + user + " " + channel + " :is already on channel"

#define ERR_NOTREGISTERED(nick) ":ft_irc 451 " + nick + " :You have not registered"

#define ERR_NEEDMOREPARAMS(nick, command) ":ft_irc 461 " + nick + " " + command + " :Not enough parameters"

#define ERR_ALREADYREGISTRED(nick) ":ft_irc 462 " + nick + " :You may not reregister"

#define ERR_PASSWDMISMATCH(nick) ":ft_irc 464 " + nick + " :Password incorrect"

#define ERR_CHANNELISFULL(nick, channel) ":ft_irc 471 " + nick + " " + channel + " :Cannot join channel (+l)"

#define ERR_INVITEONLYCHAN(nick, channel) ":ft_irc 473 " + nick + " " + channel + " :Cannot join channel (+i)"

#define ERR_BADCHANNELKEY(nick, channel) ":ft_irc 475 " + nick + " " + channel + " :Cannot join channel (+k)"

#define ERR_CHANOPRIVSNEEDED(nick, channel) ":ft_irc 482 " + nick + " " + channel + " :You're not channel operator"

#define ERR_UNKNOWNMODE(nick, mode) ":ft_irc 472 " + nick + " " + mode + " :is unknown mode char to me"

#define ERR_UNKNOWNCOMMAND(nick, command) ":ft_irc 421 " + nick + " " + command + " :Unknown command"

#define RPL_NAMREPLY(nick, channel, names) ":ft_irc 353 " + nick + " = " + channel + " :" + names

// #define RPL_ENDOFNAMES( nick, channel) ":ft_irc 366 " + nike + " " + channel + " :End of /NAMES list"

#define RPL_ENDOFNAMES( nick, channel) ":ft_irc 366 " + nick + " " + channel + " :End of /NAMES list"


#endif