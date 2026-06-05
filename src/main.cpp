#include "../include/Client.hpp"
#include "../include/Server.hpp"

int main()
{
    Server server;
    Client client;
    server.handleCommand(&client, "PASS secret");
    server.handleCommand(&client, "NICK mehdi");
    server.handleCommand(&client, "USER mehdi 0 * :Elmehdi elkabia");

    
    return 0;
}