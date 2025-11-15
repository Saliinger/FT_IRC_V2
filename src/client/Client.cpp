#include "../include/Client.hpp"
#include "../include/Channel.hpp"
#include "../include/Utils.hpp"

#include <iostream>
#include <string>

// Constructor & Destructor
Client::Client(int fd)
    :   _fd(fd),
        _isAuth(false),
        _isRegistered(false)
{ std::cout   << "Client constructor called" << std::endl; }

Client::~Client()
{ std::cout << "Client destructor called" << std::endl; }

// Getters
int Client::getFd() const
{ return (_fd); }

bool Client::isAuthenticated() const
{ return (_isAuth); }

bool Client::isRegistered() const
{ return (_isRegistered); }

const std::string&  Client::getNickname() const
{ return (_nickname); }

const std::string&  Client::getUsername() const
{ return (_username); }

const std::string&  Client::getIpAdress() const
{ return (_ipAdress); }

const std::map<std::string, Channel *>& Client::getChannelList() const
{ return (_channelList); }

// Setters
void    Client::setNickname(const std::string& nickname)
{ _nickname = nickname; }

void    Client::setUsername(const std::string& username)
{ _username = username; }

void    Client::setIpAdress(const std::string& ipAdress)
{ _ipAdress = ipAdress; }

//Methodes
void    Client::authenticate()
{ _isAuth = true; }

void    Client::unAuthenticate()
{ _isAuth = false; }

void    Client::registerClient()
{ _isRegistered = true; }

void    Client::appendToRecvBuffer(const std::string& data)
{ _recvBuffer += data; }

bool    Client::hasCompleteMessage()
{ return (_recvBuffer.find("\r\n") != std::string::npos); }

std::string Client::extractMessage()
{
    size_t  pos = _recvBuffer.find("\r\n");
    if (pos == std::string::npos)
        return ("");
    std::string message = _recvBuffer.substr(0, pos + 2);
    _recvBuffer.erase(0, pos + 2);
    return (message);
}

void    Client::joinChannel(Channel* channel)
{
    std::cout << "Client: " << _username << " joined channel: " << channel->getChannelName() << std::endl;
    _channelList[channel->getChannelName()] = channel;
    channel->addClient(this);
}

void    Client::leaveChannel(Channel* channel)
{
    std::cout << "Client: " << _username << " left channel: " << channel->getChannelName() << std::endl;
    _channelList.erase(channel->getChannelName());
    channel->removeClient(this);
}

void    Client::sendMessage(const std::string& message)
{ sendToClient(_fd, message); }