#include "../../include/PartCommand.hpp"
#include "../../include/Server.hpp"
#include "../../include/Client.hpp"
#include "../../include/Channel.hpp"

void PartCommand::execute(Server &server, Client &client, const std::vector<std::string> &args)
{
	(void)server;
	(void)client;
	(void)args;

	std::vector<Channel *> channels;
	std::string msg;

	for (std::vector<std::string>::const_iterator it = args.begin(); it != args.end(); it++)
	{
		if ((*it)[0] == '#')
		{
			Channel *to_add = server.getChannel(*it);
			channels.push_back(to_add);
		}
		else
			msg = *it;
	}

	std::string fullMsg = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getIpAdress() + " PART ";

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