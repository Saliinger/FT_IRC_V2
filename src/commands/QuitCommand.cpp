#include "../../include/QuitCommand.hpp"
#include "../../include/Server.hpp"
#include "../../include/Client.hpp"
#include "../../include/Channel.hpp"
#include <unistd.h>

void QuitCommand::execute(Server &server, Client &client, const std::vector<std::string> &args)
{
	std::string reason = args.empty() ? "Quit" : args[0];
	std::string quitMsg = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getIpAdress() + " QUIT :" + reason + "\r\n";

	const std::map<std::string, Channel *> &channels = client.getChannelList();
	for (std::map<std::string, Channel *>::const_iterator itc = channels.begin(); itc != channels.end(); ++itc)
	{
		itc->second->sendMessageToClients(client.getFd(), quitMsg);
	}

	client.sendMessage(quitMsg);
	close(client.getFd());

	// add to departed
	server.addDeparted(client);
	server.removeClient(client);
}

// quit the irc serv with or without a reason
// so unauth + close fd;
// reason + quiting notification receive in all channels that the client is in
// do not leave any channel just qui the serv

// in case of reconnect delete the old client and recreate one at the new fd position in the map