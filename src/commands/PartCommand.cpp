#include "../../include/PartCommand.hpp"

void PartCommand::execute(Server &server, Client &client, const std::vector<std::string> &args)
{
	(void)client;

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

	msg += "\r\n";

	for (std::vector<Channel *>::iterator it = channels.begin(); it != channels.end(); it++)
	{

		(*it)->sendMessage(msg);
	}
}

// leave one or more channel + whith or whithout a reason

// split the channels and msg
// get channel by channel and send the msg if it exist