#include "../include/Bot.hpp"
#include "../include/Server.hpp"

Bot::Bot( void ) : _name("Slave") {}

Bot::Bot( const Bot &old ) : _name(old._name) {}

Bot	&Bot::operator=( const Bot &old )
{
	if ( this != &old )
		_name = old._name;

	return (*this);
}

const std::string	&Bot::getName( void ) const
{
	return (_name);
}

const std::string	Bot::randomJokes( void )
{
	std::string	joke1 = "If two vegetarians get in an argument, is it still called beef ?\n";

	std::string	joke2 = "What do you call a pregnant slave?\nBuy one, get one free.\n";

	std::string	joke3 = "Boy: Grandpa, what's dark humor?\n";
	joke3 += "Grandpa: You see that guy over there with no arms? Go tell him to clap.\n";
	joke3 += "Boy: But grandpa, i'm blind.\n";

	std::string	joke4 = "Two things that never get old...\nDark humor & unvaccinated children\n";

	std::string	joke5 = "Kid: Will you visit me when i get out ?\n";
	joke5 += "Nurse: lol nuh, I hate graveyards\n";

	std::string	joke6 = "My doctor told me I had six months to live.\nI couldn't pay the bill, so he gave me another six months.\n";

	int			random = rand() % 6;
	std::string	arr[6] = {joke1, joke2, joke3, joke4, joke5, joke6};

	return (arr[random]);
}

std::string trim(const std::string &str)
{
    size_t start = str.find_first_not_of(" \t");
    if (start == std::string::npos)
        return "";

    size_t end = str.find_last_not_of(" \t");

    return (str.substr(start, end - start + 1));
}

void				Bot::handleMessage( Server &server, Client &client, const std::string &msg )
{
	std::string	message;
	std::string	command	= trim(msg);

	if ( command == "!ping" )
	{
		message = "\033[32m";
		message += getName() + ": \033[0m";
		message += "pong\n";
	}
	else if ( command == "!time" )
	{
		time_t		now = time(NULL);
		std::string	s_time = ctime(&now);

		message = "\033[32m";
		message += getName() + ": \033[0m";
		message += s_time;
	}
	else if ( command == "!dice" )
	{
		int					dice = rand() % 6 + 1;
		std::stringstream	s_dice;

		s_dice << dice;

		message = "\033[32m";
		message += getName() + ": \033[0m";
		message += "🎲 " + s_dice.str() + "\n";
	}
	else if ( command == "!help" )
	{
		message = "\033[32m";
		message += getName() + ": \033[0m";
		message += "Available commands:\n";
		message += "!ping\n!dice\n!time\n!users\n!channels\n!joke\n!help\nHi!\nHello!\nThanks!\nBye!\n";
	}
	else if ( command == "!users" )
	{
		int					users = server.clientSize();
		std::stringstream	s_users;

		s_users << users;
		message = "\033[32m";
		message += getName() + ": \033[0m";
		message += "The server has " + s_users.str() + " users:\n" + server.clientsName();
	}
	else if ( command == "!channels" )
	{
		int					channel_size = server.channelSize();
		std::stringstream	s_channels_size;

		s_channels_size << channel_size;
		message += "The server has " + s_channels_size.str() + " channels:\n" + server.channelsName();
	}
	else if ( command == "!joke" )
	{
		message = "\033[32m";
		message += getName() + ": \033[0m";
		message += randomJokes();
	}
	else if ( command == "Hi!" )
	{
		message = "\033[32m";
		message += getName() + ": \033[0m";
		message += "Hi " + client.getNickname() + "!\n";
	}
	else if ( command == "Hello!" )
	{
		message = "\033[32m";
		message += getName() + ": \033[0m";
		message += "Hello " + client.getNickname() + "!\n";
	}
	else if ( command == "Thanks!" )
	{
		message = "\033[32m";
		message += getName() + ": \033[0m";
		message += "You're welcome!\n";
	}
	else if ( command == "Bye!" )
	{
		message = "\033[32m";
		message += getName() + ": \033[0m";
		message += "See you later!\n";
	}
	else
	{
		message = "\033[32m";
		message += getName() + ": \033[0m";
		message += "I'm too lazy to chat right now :D But you can try !help to see the commands I'm not too lazy to answer. lol\n";
	}

	server.sendToClient( &client, message );
}

Bot::~Bot( void ) {}


