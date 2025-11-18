#include "../../include/PingCommand.hpp"
#include "../../include/Client.hpp"
#include "../../include/Server.hpp"

#include <iostream>

void PingCommand::execute(Server &server, Client &client, const std::vector<std::string> &args)
{
    (void)server;

    if (args.empty())
    {
        client.sendMessage(":ft_irc 409 " + client.getNickname() + " :No origin specified\r\n");
        return;
    }

    std::string token = args[0];
    client.sendMessage(":ft_irc PONG ft_irc :" + token + "\r\n");
}
