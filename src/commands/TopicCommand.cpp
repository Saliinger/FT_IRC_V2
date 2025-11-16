#include "../../include/TopicCommand.hpp"

void TopicCommand::execute(Server &server, Client &client, const std::vector<std::string> &args)
{
	if (args.size() == 1)
	{
		client.sendMessage(formatError(ERR_NEEDMOREPARAMS, client.getNickname(), "", "not enough parameters"));
		return;
	}

	std::string clientName = client.getNickname();
	if (!client.isRegistered())
	{
		client.sendMessage(formatError(ERR_NOTREGISTERED, clientName.empty() ? "*" : clientName, "", "you're not registered"));
		return;
	}

	std::vector<std::string>::const_iterator it = args.begin();
	Channel *channel = server.getChannel(*it);

	if (!channel)
	{
		client.sendMessage(formatError(ERR_NOSUCHCHANNEL, client.getNickname(), channel->getChannelName(), "channel doesn't exist"));
		return;
	}

	// if (channel->)
	// client.sendMessage(formatError(ERR_NOTONCHANNEL, client.getNickname(), channel->getChannelName(), "not part of the channel"));

	it++;
	if (it == args.end())
	{
		std::string topic = channel->getTopic();
		if (topic.empty())
			client.sendMessage("no topic");
		else
			client.sendMessage(topic);
		return;
	}

	if (channel->getChannelMode(MODE_T))
	{
		if (!channel->isOperator(&client))
		{
			client.sendMessage(formatError(ERR_CHANOPRIVSNEEDED, client.getNickname(), channel->getChannelName(), "need to be operator"));
			return;
		}
	}
	channel->setTopic(*it);

	std::string topicMsg = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getIpAdress() +
						   " TOPIC " + channel->getChannelName() + " :" + channel->getTopic() + "\r\n";
	channel->sendMessageToClients(-1, topicMsg);
}