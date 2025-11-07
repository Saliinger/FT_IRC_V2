#include "../../include/NickCommand.hpp"
#include "../../include/Server.hpp"
#include "../../include/Client.hpp"

#include <iostream>
#include <cctype>

static bool isValidNickname(const std::string &nick)
{
    if (nick.empty() || nick.length() > 9)
        return (false);
    if (!std::isalpha(nick[0]) && nick[0] != '[' && nick[0] != ']' && 
        nick[0] != '{' && nick[0] != '}' && nick[0] != '\\' && 
        nick[0] != '|' && nick[0] != '_' && nick[0] != '^')
        return (false);
    for (size_t i = 1; i < nick.length(); ++i)
    {
        char c = nick[i];
        if (!std::isalnum(c) && c != '-' && c != '[' && c != ']' && 
            c != '{' && c != '}' && c != '\\' && c != '|' && c != '_' && c != '^')
            return (false);
    }
    return (true);
}

void NickCommand::execute(Server &server, Client &client, const std::vector<std::string> &args) 
{
    if (args.empty())
    {
        client.sendMessage(":server 431 * :No nickname given");
        return;
    }
    
    std::string newNick = args[0];
    
    if (!isValidNickname(newNick))
    {
        client.sendMessage(":server 432 * " + newNick + " :Erroneous nickname");
        return;
    }
    if (server.isNicknameUsed(newNick))
    {
        client.sendMessage(":server 433 * " + newNick + " :Nickname is already in use");
        return;
    }
    std::string oldNick = client.getNickname();
    
    if (!oldNick.empty())
    {
        client.sendMessage(":" + oldNick + "!" + client.getUsername() + "@" + 
                          client.getIpAdress() + " NICK :" + newNick);
    }
    client.setNickname(newNick);
}