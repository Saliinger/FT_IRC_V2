#pragma once

#include "./Command.hpp"
#include "Channel.hpp"
#include "Server.hpp"

#include <vector>
#include <string>

class Server;
class Channel;

class PrivmsgCommand : public Command
{
public:
	void execute(Server &server, Client &client, const std::vector<std::string> &args);
};
