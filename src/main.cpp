#include "../include/Client.hpp"
#include "../include/Server.hpp"

int main()
{
    Server server;
    Client c1;
    Client c2;
    // server.handleCommand(&c1, "PASS secret");
    // server.handleCommand(&c1, "NICK mehdi");
    // server.handleCommand(&c1, "USER mehdi 0 * :Elmehdi elkabia");

    server.addClient(&c1);
    server.addClient(&c2);

    server.handleCommand(&c1, "PASS secret");
    server.handleCommand(&c1, "NICK mehdi");
    server.handleCommand(&c1, "USER mehdi 0 * :Mehdi");

    server.handleCommand(&c1, "JOIN #42");

    server.handleCommand(&c2, "PASS secret");
    server.handleCommand(&c2, "NICK ali");
    server.handleCommand(&c2, "USER ali 0 * :Ali");

    server.handleCommand(&c2, "JOIN #42");

    // server.handleCommand(&c1, "PRIVMSG #42 :Hello everyone!");
    server.handleCommand(&c1, "PRIVMSG ali :hello");

    // server.handleCommand(&c1, "PART #42");

    return 0;
}