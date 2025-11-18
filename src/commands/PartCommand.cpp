#include "../../include/PartCommand.hpp"
#include "../../include/Channel.hpp"
#include "../../include/Client.hpp"
#include "../../include/Server.hpp"

void PartCommand::execute(Server &server, Client &client, const std::vector<std::string> &args)
{
    std::string clientName = client.getNickname();
    if (!client.isRegistered())
    {
        client.sendMessage(
            formatError(ERR_NOTREGISTERED, clientName.empty() ? "*" : clientName, "", "you're not registered"));
        return;
    }

    if (args.empty())
    {
        client.sendMessage(formatError(ERR_NEEDMOREPARAMS, client.getNickname(), "PART", "Not enough parameters"));
        return;
    }

    std::vector<Channel *> channels;
    std::string msg;

    for (std::vector<std::string>::const_iterator it = args.begin(); it != args.end(); it++)
    {
        if ((*it)[0] == '#')
        {
            Channel *to_add = server.getChannel(*it);
            if (!to_add)
            {
                client.sendMessage(formatError(ERR_NOSUCHCHANNEL, client.getNickname(), *it, "channel doesn't exist"));
                continue;
            }
            if (!to_add->isClient(&client))
            {
                client.sendMessage(formatError(ERR_NOTONCHANNEL, client.getNickname(), *it, "not part of the channel"));
                continue;
            }
            channels.push_back(to_add);
        }
        else
            msg = *it;
    }

    std::string fullMsg =
        ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getIpAdress() + " PART ";

    for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
    {
        std::string channelMsg = fullMsg + (*it)->getChannelName();
        if (!msg.empty())
            channelMsg += " :" + msg;
        channelMsg += "\r\n";

        (*it)->sendMessageToClients(-1, channelMsg);
        client.leaveChannel(*it);
    }

    // quit each specified channel
}

// leave one or more channel + whith or whithout a reason

// split the channels and msg
// get channel by channel and send the msg if it exist