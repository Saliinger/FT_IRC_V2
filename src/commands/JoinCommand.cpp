#include "../../include/JoinCommand.hpp"
#include "../../include/Server.hpp"
#include "../../include/Client.hpp"
#include "../../include/Channel.hpp"

#include <iostream>
#include <sstream>

void JoinCommand::execute(Server &server, Client &client, const std::vector<std::string> &args)
{
	if (!client.isRegistered())
	{
		client.sendMessage(":localhost 451 * :You have not registered\r\n");
		return;
	}

	if (args.empty())
	{
		client.sendMessage(":localhost 461 " + client.getNickname() + " JOIN :Not enough parameters\r\n");
		return;
	}

	std::string channelName = args[0];
	std::string channelKey = args.size() > 1 ? args[1] : "";

	if (channelName.empty() || (channelName[0] != '#' && channelName[0] != '&'))
	{
		client.sendMessage(":localhost 403 " + client.getNickname() + " " + channelName + " :No such channel\r\n");
		return;
	}

	std::map<std::string, Channel *> channels = server.getChannels();
	Channel *channel = NULL;

	std::map<std::string, Channel *>::iterator it = channels.find(channelName);
	if (it != channels.end())
	{
		channel = it->second;

		if (!channel->getPassword().empty() && channel->getPassword() != channelKey)
		{
			client.sendMessage(":localhost 475 " + client.getNickname() + " " + channelName + " :Cannot join channel (+k)\r\n");
			return;
		}
	}
	else
	{
		channel = new Channel(channelName);
		// add the new channel to the map
		server.addChannel(channel);
	}

	client.joinChannel(channel);
	client.sendMessage(":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getIpAdress() + " JOIN :" + channelName + "\r\n");

	if (!channel->getTopic().empty())
		client.sendMessage(":localhost 332 " + client.getNickname() + " " + channelName + " :" + channel->getTopic() + "\r\n");
	else
		client.sendMessage(":localhost 331 " + client.getNickname() + " " + channelName + " :No topic is set\r\n");

	std::string namesList = ":localhost 353 " + client.getNickname() + " = " + channelName + " :";

	const std::map<int, Client *> &clients = channel->getClients();
	for (std::map<int, Client *>::const_iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (channel->isOperator(it->second))
			namesList += "@";
		namesList += it->second->getNickname() + " ";
	}
	namesList += "\r\n";

	client.sendMessage(namesList);
	client.sendMessage(":localhost 366 " + client.getNickname() + " " + channelName + " :End of /NAMES list\r\n");
	channel->sendMessageToClients(client.getFd(), ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getIpAdress() + " JOIN :" + channelName + "\r\n");
}
