#pragma once

#include <string>
#include <vector>

#include "Client.hpp"
#include "Server.hpp"

class Server;
class Client;

class Command {
	public:
		virtual ~Command();
		virtual void execute(Server &server, Client &client, const std::vector<std::string> &args) = 0;
};