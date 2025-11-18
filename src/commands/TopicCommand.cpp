#include "../../include/TopicCommand.hpp"

void TopicCommand::execute(Server &server, Client &client, const std::vector<std::string> &args)
{
    if (args.empty())
    {
        client.sendMessage(formatError(ERR_NEEDMOREPARAMS, client.getNickname(), "", "not enough parameters"));
        return;
    }

    std::string clientName = client.getNickname();
    if (!client.isRegistered())
    {
        client.sendMessage(
            formatError(ERR_NOTREGISTERED, clientName.empty() ? "*" : clientName, "", "you're not registered"));
        return;
    }

    std::vector<std::string>::const_iterator it = args.begin();

    if (!server.channelExist(*it))
    {
        client.sendMessage(formatError(ERR_NOSUCHCHANNEL, client.getNickname(), *it, "channel doesn't exist"));
        return;
    }
    Channel *channel = server.getChannel(*it);

    if (!channel->isClient(&client))
    {
        client.sendMessage(
            formatError(ERR_NOTONCHANNEL, client.getNickname(), channel->getChannelName(), "not part of the channel"));
        return;
    }

    it++;
    if (it == args.end())
    {
        std::string topic = channel->getTopic();
        if (topic.empty())
            client.sendMessage(
                formatReply(RPL_NOTOPIC, client.getNickname(), channel->getChannelName() + " :No topic is set"));
        else
            client.sendMessage(formatReply(RPL_TOPIC, client.getNickname(), channel->getChannelName() + " :" + topic));
        return;
    }

    if (channel->getChannelMode(MODE_T))
    {
        if (!channel->isOperator(&client))
        {
            client.sendMessage(formatError(ERR_CHANOPRIVSNEEDED, client.getNickname(), channel->getChannelName(),
                                           "You're not channel operator"));
            return;
        }
    }
    channel->setTopic(*it);

    std::string topicMsg = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getIpAdress() +
                           " TOPIC " + channel->getChannelName() + " :" + channel->getTopic() + "\r\n";
    channel->sendMessageToClients(-1, topicMsg);
}