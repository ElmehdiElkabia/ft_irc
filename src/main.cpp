#include "../include/Client.hpp"
#include "../include/Server.hpp"

int main( int ac, char**  av )
{
	Server				ser;
	int					port;
	std::string			password;

	std::cout << "---- SERVER ----" << std::endl;

	if ( ac != 3 )
	{
		std::cerr << "Error: Usage: ./ircserv <port> <password>" << std::endl;
		return (1);
	}

	std::stringstream	ss(av[1]);

	if ( !(ss >> port) )
	{
		std::cerr << "Error: Ports like 6767 | 6667 ...etc" << std::endl;
		return (1);
	}

	if ( port < 1024 || port > 65535 )
	{
		std::cerr << "Error: Try a port from this range [1024, 65535]" << std::endl;
		return (1);
	}

	password = av[2];
	
	try{
		signal(SIGINT, Server::SignalHandler); //-> catch the signal (ctrl + c)
		signal(SIGQUIT, Server::SignalHandler); //-> catch the signal (ctrl + \)
		ser.ServerInit( port, password ); //-> initialize the server
	}
	catch(const std::exception& e){
		ser.CloseFds(); //-> close the file descriptors
		std::cerr << e.what() << std::endl;
	}

	std::cout << "The Server Closed!" << std::endl;
}