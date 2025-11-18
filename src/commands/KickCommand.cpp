#include "../../include/KickCommand.hpp"
#include "../../include/Channel.hpp"
#include "../../include/Client.hpp"
#include "../../include/Server.hpp"

#include <iostream>

void KickCommand::execute(Server &server, Client &client, const std::vector<std::string> &args)
{
    if (!client.isRegistered())
    {
        client.sendMessage(":localhost 451 * :You have not registered\r\n");
        return;
    }

    if (args.size() < 2)
    {
        client.sendMessage(":localhost 461 " + client.getNickname() + " KICK :Not enough parameters\r\n");
        return;
    }

    std::string channelName = args[0];
    std::string targetNick = args[1];
    std::string reason = args.size() > 2 ? args[2] : client.getNickname();

    Channel *channel = server.getChannel(channelName);
    if (!channel)
    {
        client.sendMessage(formatError(ERR_NOSUCHCHANNEL, client.getNickname(), channelName, "don't exist"));
        return;
    }

    const std::map<int, Client *> &clients = channel->getClients();

    if (clients.find(client.getFd()) == clients.end())
    {
        client.sendMessage(":localhost 442 " + client.getNickname() + " " + channelName +
                           " :You're not on that channel\r\n");
        return;
    }

    if (!channel->isOperator(&client))
    {
        client.sendMessage(":localhost 482 " + client.getNickname() + " " + channelName +
                           " :You're not channel operator\r\n");
        return;
    }

    Client *targetClient = NULL;
    for (std::map<int, Client *>::const_iterator clientIt = clients.begin(); clientIt != clients.end(); ++clientIt)
    {
        if (clientIt->second->getNickname() == targetNick)
        {
            targetClient = clientIt->second;
            break;
        }
    }

    if (!targetClient)
    {
        client.sendMessage(":localhost 441 " + client.getNickname() + " " + targetNick + " " + channelName +
                           " :They aren't on that channel\r\n");
        return;
    }

    std::string kickMsg = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getIpAdress() +
                          " KICK " + channelName + " " + targetNick + " :" + reason + "\r\n";
    for (std::map<int, Client *>::const_iterator clientIt = clients.begin(); clientIt != clients.end(); ++clientIt)
        clientIt->second->sendMessage(kickMsg);
    targetClient->leaveChannel(channel);
}
