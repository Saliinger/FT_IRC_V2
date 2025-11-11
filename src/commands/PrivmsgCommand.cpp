#include "../../include/PrivmsgCommand.hpp"

void PrivmsgCommand::execute(Server &server, Client &client, const std::vector<std::string> &args)
{
	// msg between people
	// msg to channel
	// cmd nick :msg
	Channel *targetChannel = server.getChannel(args[1]);
	std::string msg(args[1] + "'\r\n");
	targetChannel->sendMessageToClients(msg);
}