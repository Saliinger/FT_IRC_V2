#pragma once

#include <map>
#include <string>
#include <vector>

#include "Command.hpp"
#include "Server.hpp"

class Command;
class Server;

class CommandHandler
{
private:
	std::map<std::string, Command *> _commands;

public:
	CommandHandler();
	~CommandHandler();

	void handleCommand(Server &server, Client &client, const std::string &input);
};