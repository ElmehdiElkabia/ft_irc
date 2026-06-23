#ifndef CLIENT_HPP
#define CLIENT_HPP


#include <iostream>
#include <sstream>


class Client
{
    private:
        int         fd;
        std::string IPadd;
        std::string nickname;
        std::string username;
        std::string realname;
        std::string hostname;
        bool        passAccepted;
        bool        registered;

    public:
        Client();
        Client(int fd);
        ~Client();

        int getFd() const;
        
        void setFd(int fd){this->fd = fd;}
        void setIpAdd(std::string ipadd){this->IPadd = ipadd;}
        void setHostname(std::string hostname){this->hostname = hostname;}
        
        const std::string& getNickname() const;
        const std::string& getUsername() const;
        std::string getHostname() const;

        bool isRegistered() const;
        bool hasPassed() const;

        void setNickname(const std::string& nick);
        void setUsername(const std::string& user);
        void setRealname(const std::string& real);
        void setPassAccepted(bool value);
        void setRegistered(bool value);
};


#endif