#include "../../include/InviteCommand.hpp"
#include "../../include/Channel.hpp"
#include "../../include/Client.hpp"
#include "../../include/Server.hpp"

#include <iostream>

void InviteCommand::execute(Server &server, Client &client, const std::vector<std::string> &args)
{
    if (!client.isRegistered())
    {
        client.sendMessage(":ft_irc 451 * :You have not registered\r\n");
        return;
    }

    if (args.size() < 2)
    {
        client.sendMessage(":ft_irc 461 " + client.getNickname() + " INVITE :Not enough parameters\r\n");
        return;
    }

    std::string targetNick = args[0];
    std::string channelName = args[1];

    Channel *channel = server.getChannel(channelName);
    if (!channel)
    {
        client.sendMessage(formatError(ERR_NOSUCHCHANNEL, client.getNickname(), channelName, "don't exist"));
        return;
    }

    const std::map<int, Client *> &clients = channel->getClients();
    if (clients.find(client.getFd()) == clients.end())
    {
        client.sendMessage(":ft_irc 442 " + client.getNickname() + " " + channelName +
                           " :You're not on that channel\r\n");
        return;
    }

    Client *targetClient = server.getClient(targetNick);

    if (!targetClient)
    {
        client.sendMessage(":ft_irc 401 " + client.getNickname() + " " + targetNick + " :No such nick/channel\r\n");
        return;
    }

    if (channel->isClient(targetClient))
    {
        client.sendMessage(":ft_irc 443 " + client.getNickname() + " " + targetNick + " " + channelName +
                           " :is already on channel\r\n");
        return;
    }

	channel->addToInviteList(targetClient);
    targetClient->sendMessage(":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getIpAdress() +
                              " INVITE " + targetNick + " :" + channelName + "\r\n");
    client.sendMessage(":ft_irc 341 " + client.getNickname() + " " + targetNick + " " + channelName + "\r\n");
}
