# ifndef BOT_HPP
# define BOT_HPP

#include <iostream>
#include <string>
#include <sstream>
#include <ctime>
#include <cstdlib>

class Server;
class Client;

class	Bot
{
	private:
		std::string _name;

	public:
		Bot( void );
		Bot( const Bot &old );
		Bot &operator=( const Bot &old );
		~Bot( void );

		const std::string	&getName( void ) const;
		void				handleMessage( Server &server, Client &client, const std::string &msg );
		const std::string	randomJokes( void );
};

std::string		trim(const std::string &str);


# endif