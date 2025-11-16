#pragma once

#include <string>
#include <map>

class Channel;

class Client
{
    private:
        int                                     _fd;
        bool                                    _isAuth;
        bool                                    _isRegistered;
        std::string                             _nickname;
        std::string                             _username;
        std::string                             _ipAdress;
        std::string                             _recvBuffer;
        std::map<std::string, Channel *>        _channelList;

    public:
        Client(int fd);
        ~Client();

        // Getters & Setters
        int                                     getFd() const;
        bool                                    isAuthenticated() const;
        bool                                    isRegistered() const;
        const std::string                       &getNickname() const;
        const std::string                       &getUsername() const;
        const std::string                       &getIpAdress() const;
        const std::map<std::string, Channel *>  &getChannelList() const;
        
        void                                    setNickname(const std::string &nickname);
        void                                    setUsername(const std::string &username);
        void                                    setIpAdress(const std::string &ipAdress);

        // Methodes
        void                                    authenticate();
        void                                    unAuthenticate();
        void                                    registerClient();
        void                                    appendToRecvBuffer(const std::string &data);
        bool                                    hasCompleteMessage();
        std::string                             extractMessage();
        void                                    joinChannel(Channel *channel);
        void                                    leaveChannel(Channel *channel);
        void                                    sendMessage(const std::string &message);
};
