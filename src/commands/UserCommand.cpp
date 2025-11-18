#include "../../include/UserCommand.hpp"
#include "../../include/Client.hpp"
#include "../../include/Server.hpp"

#include <iostream>

void UserCommand::execute(Server &server, Client &client, const std::vector<std::string> &args)
{
    (void)server;

    if (args.size() < 4)
    {
        client.sendMessage(":ft_irc 461 * USER :Not enough parameters\r\n");
        return;
    }
    if (client.isRegistered())
    {
        client.sendMessage(":ft_irc 462 " + client.getNickname() + " :You may not reregister\r\n");
        return;
    }
    std::string username = args[0];
    std::string realname = args[3];

    if (!realname.empty() && realname[0] == ':')
        realname = realname.substr(1);
    client.setUsername(username);
    if (!client.getNickname().empty() && client.isAuthenticated())
    {
        client.registerClient();
        client.sendMessage(":ft_irc 001 " + client.getNickname() + " :Welcome to the Internet Relay Network " +
                           client.getNickname() + "!" + client.getUsername() + "@" + client.getIpAdress() + "\r\n");
        client.sendMessage(":ft_irc 002 " + client.getNickname() + " :Your host is ft_irc, running version 1.0\r\n");
        client.sendMessage(":ft_irc 003 " + client.getNickname() + " :This server was created today\r\n");
        client.sendMessage(":ft_irc 004 " + client.getNickname() + " ft_irc 1.0 io itkol\r\n");
    }
}
