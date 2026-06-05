#ifndef UTILS_HPP
#define UTILS_HPP

#include <vector>
#include <iostream>

struct Command
{
    std::string command;
    std::vector<std::string> params;
};



Command parseCommand(const std::string& commandLine);

#endif 