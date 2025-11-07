#pragma once 

#include "Server.hpp"
#include "Client.hpp"
#include "CommandHandler.hpp"

class Command {
	public:
		Command(Server &Server);
		Command(const Command &src);
		Command &operator=(const Command &src);
		~Command();

	private:
		Server &server;
};