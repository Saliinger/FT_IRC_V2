#include "../../include/PrivmsgCommand.hpp"

static int getType(const std::string &to_check)
{
	if (to_check[0] == '#')
		return 1;
	return 0;
}

void PrivmsgCommand::execute(Server &server, Client &client, const std::vector<std::string> &args)
{
	std::string msg(args[2] + "\r\n");
	if (getType(args[1])) // type 1 channel
	{
		Channel *target = server.getChannel(args[1]);
		if (!target)
		{
			std::cout << "Error: channel error" << std::endl;
			return;
		}
		target->sendMessageToClients(client.getFd(), msg);
	}
	else // autre type msg vers nickname
	{
		Client *target = server.getClient(args[1]);
		if (!target)
		{
			std::cout << "Error: channel error" << std::endl;
			return;
		}
		target->sendMessage(msg);
	}
}

// check si # -> channel sinon nickname