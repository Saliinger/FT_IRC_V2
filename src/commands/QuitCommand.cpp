#include "../../include/QuitCommand.hpp"

void QuitCommand::execute(Server &server, Client &client, const std::vector<std::string> &args)
{
	client.isAuthenticated(); // logout
	close(client.getFd());

	Channel *channels = client.getChannelList();
	for (std::map<std::string, Channel *>::iterator itc = channels.begin(); it != channels.end(); it++)
		it->sendMessage(args[0] + "\r\n");
	
	// add to departed
	server.addDeparted(client);
	server.removeClient(client);
}

// quit the irc serv with or without a reason
// so unauth + close fd;
// reason + quiting notification receive in all channels that the client is in
// do not leave any channel just qui the serv

// in case of reconnect delete the old client and recreate one at the new fd position in the map