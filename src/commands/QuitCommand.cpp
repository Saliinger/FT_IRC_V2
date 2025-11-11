#include "../../include/QuitCommand.hpp"

void QuitCommand::execute(Server &server, Client &client, const std::vector<std::string> &args)
{
}

// quit the irc serv with or without a reason
// so unauth + close fd;