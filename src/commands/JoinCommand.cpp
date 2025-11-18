#include "../../include/JoinCommand.hpp"
#include "../../include/Channel.hpp"
#include "../../include/Client.hpp"
#include "../../include/Server.hpp"

#include <iostream>
#include <sstream>

void JoinCommand::execute(Server &server, Client &client, const std::vector<std::string> &args)
{
    std::string clientName = client.getNickname();
    if (!client.isRegistered())
    {
        client.sendMessage(
            formatError(ERR_NOTREGISTERED, clientName.empty() ? "*" : clientName, "", "You have not registered"));
        return;
    }

    if (args.empty())
    {
        client.sendMessage(formatError(ERR_NEEDMOREPARAMS, client.getNickname(), "JOIN", "Not enough parameters"));
        return;
    }

    std::string channelName = args[0];
    std::string channelKey = args.size() > 1 ? args[1] : "";

    if (channelName.empty() || (channelName[0] != '#' && channelName[0] != '&'))
    {
        client.sendMessage(formatError(ERR_NOSUCHCHANNEL, client.getNickname(), channelName, "No such channel"));
        return;
    }

    Channel *channel = server.getChannel(channelName);
    if (channel)
    {
        if (!channel->getPassword().empty() && channel->getPassword() != channelKey)
        {
            client.sendMessage(
                formatError(ERR_BADCHANNELKEY, client.getNickname(), channelName, "Cannot join channel (+k)"));
            return;
        }
        // Channel exists, check restrictions
        if (channel->getChannelMode(MODE_I))
        {
            if (channel->isInvited(&client))
            {
                if (!client.joinChannel(channel))
                {
                    client.sendMessage(
                        formatError(ERR_CHANNELISFULL, client.getNickname(), channelName, "Cannot join channel (+l)"));
                    return;
                }
                channel->removeFromInviteList(&client);
            }
            client.sendMessage(
                formatError(ERR_INVITEONLYCHAN, client.getNickname(), channelName, "Cannot join channel (+i)"));
            return;
        }
    }
    else
    {
        // Channel doesn't exist, create it
        channel = new Channel(channelName);
        server.addChannel(channel);
    }

    if (!client.joinChannel(channel))
    {
        client.sendMessage(
            formatError(ERR_CHANNELISFULL, client.getNickname(), channelName, "Cannot join channel (+l)"));
        return;
    }

    // Make first user operator when creating new channel
    if (channel->getClients().size() == 1)
        channel->setOperator(&client);

    client.sendMessage(":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getIpAdress() +
                       " JOIN :" + channelName + "\r\n");

    if (!channel->getTopic().empty())
        client.sendMessage(formatReply(RPL_TOPIC, client.getNickname(), channelName + " :" + channel->getTopic()));
    else
        client.sendMessage(formatReply(RPL_NOTOPIC, client.getNickname(), channelName + " :No topic is set"));

    std::string namesList;
    const std::map<int, Client *> &clients = channel->getClients();
    for (std::map<int, Client *>::const_iterator it = clients.begin(); it != clients.end(); ++it)
    {
        if (channel->isOperator(it->second))
            namesList += "@";
        namesList += it->second->getNickname() + " ";
    }

    client.sendMessage(formatReply(RPL_NAMREPLY, client.getNickname(), "= " + channelName + " :" + namesList));
    client.sendMessage(formatReply(RPL_ENDOFNAMES, client.getNickname(), channelName + " :End of /NAMES list"));
    channel->sendMessageToClients(client.getFd(), ":" + client.getNickname() + "!" + client.getUsername() + "@" +
                                                      client.getIpAdress() + " JOIN :" + channelName + "\r\n");
}

// check channel settings like i only or client limit and password