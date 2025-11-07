#include "../../include/NickCommand.hpp"
#include "../../include/Server.hpp"
#include "../../include/Client.hpp"

#include <iostream>

void NickCommand::execute(Server &server, Client &client, const std::vector<std::string> &args) {
    if (args.empty())
    {
        client.sendMessage("431 ERR_NONICKNAMEGIVEN :No nickname given");
        return ;
    }
    std::string newNick = args[0];
    return ;
}