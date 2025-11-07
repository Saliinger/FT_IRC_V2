#include "../../include/NickCommand.hpp"
#include "../../include/Server.hpp"
#include "../../include/Client.hpp"

#include <iostream>

void NickCommand::execute(Server &server, Client &client, const std::vector<std::string> &args) 
{
    if (args.empty())
    {
        client.sendMessage("431 ERR_NONICKNAMEGIVEN :No nickname given");
        return ;
    }
    std::string newNick = args[0];
    if (server.isNicknameUsed(newNick))
    {
        client.sendMessage("433 ERR_NICKNAMEINUSE " + newNick + " :Nickname is already in use");
        return;
    }
    client.setNickname(newNick);
    client.sendMessage(":" + newNick + " NICK " + newNick);
}