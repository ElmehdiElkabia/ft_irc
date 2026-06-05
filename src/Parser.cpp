#include "../include/Utils.hpp"

Command parseCommand(const std::string &commandLine)
{
    Command cmd;

    size_t pos = commandLine.find(' ');
    if (pos == std::string::npos)
    {
        cmd.command = commandLine;
    }
    else
    {
        cmd.command = commandLine.substr(0, pos);
        std::string paramsStr = commandLine.substr(pos + 1);
        size_t start = 0;
        while (start < paramsStr.length())
        {
            if (paramsStr[start] == ':')
            {
                cmd.params.push_back(paramsStr.substr(start + 1));
                break;
            }
            size_t end = paramsStr.find(' ', start);
            if (end == std::string::npos)
                end = paramsStr.length();
            cmd.params.push_back(paramsStr.substr(start, end - start));
            start = end + 1;
            while (start < paramsStr.length() && paramsStr[start] == ' ')
                start++;
        }
    }
    return cmd;
}