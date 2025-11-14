#pragma once

#include <iostream>
#include <fcntl.h>

#include "./Command.hpp"
#include "Client.hpp"

class Server;
class Client;

class QuitCommand : public Command
{
public:
	void execute(Server &server, Client &client, const std::vector<std::string> &args);
};
