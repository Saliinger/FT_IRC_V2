#include "../../include/TopicCommand.hpp"

void TopicCommand::execute(Server &server, Client &client, const std::vector<std::string> &args)
{
	// check if topic is op only
	// check if the client is op in that case

	std::string clientName = client.getNickname();
	if (!client.isAuthenticated())
	{ // error unauth / unregistered client
		client.sendMessage(formatError(ERR_NOTREGISTERED, !clientName.empty() ? "un known" : clientName, "", "you're not registered"));
		return;
	}

	

	std::vector<std::string>::const_iterator it = args.begin();
	Channel *channel = server.getChannel(*it);

	if (!channel)
	{ // error no such channel
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
			// error need to be sent to the clien
		}
	}
	channel->setTopic(*it);

	std::string topicMsg = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getIpAdress() +
						   " TOPIC " + channel->getChannelName() + " :" + channel->getTopic() + "\r\n";
	channel->sendMessageToClients(-1, topicMsg);
	// need to send the updated topic
}

// 1. Check registration
// 2. Check parameters
// 3. Check channel exists
// 4. Check user is on channel
// 5. If only channel name provided - VIEW topic
// 6. Setting topic - check if +t mode is set
// 7. Set new topic and broadcast
