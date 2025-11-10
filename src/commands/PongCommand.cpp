#include "../../include/PongCommand.hpp"
#include "../../include/Server.hpp"
#include "../../include/Client.hpp"

#include <iostream>

void PongCommand::execute(Server &server, Client &client, const std::vector<std::string> &args) 
{
    (void)server;
    (void)client;
    (void)args;

	client.sendMessage(":ft_irc PONG\r\n");
}
