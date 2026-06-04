#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <vector>

struct Command
{
    std::string command;
    std::vector<std::string> params;
}



Command parseCommand(const std::string& commandLine);

#endif 