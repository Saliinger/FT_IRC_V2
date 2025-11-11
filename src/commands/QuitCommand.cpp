#include "../../include/QuitCommand.hpp"

void QuitCommand::execute(Server &server, Client &client, const std::vector<std::string> &args)
{
	(void)server;
	(void)client;
	(void)args;
}

// quit the irc serv with or without a reason
// so unauth + close fd;
// reason + quiting notification receive in all channels that the client is in
// do not leave any channel just qui the serv

// in case of reconnect delete the old client and recreate one at the new fd position in the map