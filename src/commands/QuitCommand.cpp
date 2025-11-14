#include "../../include/QuitCommand.hpp"

void QuitCommand::execute(Server &server, Client &client, const std::vector<std::string> &args)
{
	(void)server;
	client.isAuthenticated(); // logout
	std::cout << "QUIT DEBUG" << std::endl;
	for (std::vector<std::string>::const_iterator it = args.begin(); it != args.end(); it++)
		std::cout << *it << std::endl;
	close(client.getFd());
	Channel *channels = client.getChannelList();

	for (std::map<std::string, Channel *>::iterator itc = channels.begin(); it != channels.end(); it++)
		it->sendMessage("bye\r\n");
	
	// add to departed
	server.addDeparted(client);
	server.removeClient(client);
}

// quit the irc serv with or without a reason
// so unauth + close fd;
// reason + quiting notification receive in all channels that the client is in
// do not leave any channel just qui the serv

// in case of reconnect delete the old client and recreate one at the new fd position in the map