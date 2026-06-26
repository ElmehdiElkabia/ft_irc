#include "../include/Bot.hpp"
#include "../include/Server.hpp"

Bot::Bot( void ) : _name("Slave") {}

Bot::Bot( const Bot &old ) : _name(old._name) {}

Bot	&Bot::operator=( const Bot &old )
{
	if ( this == &old )
		_name = old._name;

	return (*this);
}

const std::string	&Bot::getName( void ) const
{
	return (_name);
}

void				Bot::handleMessage( Server &server, Client &client, const std::string &msg )
{
	std::string	message;

	if ( msg == "!ping" )
	{
		message = server.getBot().getName() + ": pong\n";

		server.sendToClient( &client, message );
	}
	else if ( msg == "!time" )
	{
		time_t		now = time(NULL);

		std::string	s_time = ctime(&now);

		message = server.getBot().getName() + ": " + s_time;
	}
}

Bot::~Bot( void ) {}


