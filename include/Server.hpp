#pragma once
 
#include "Client.hpp"
#include "Channel.hpp"

class Server {
	public:
		Server();
		Server(const Server &src);
		Server &operator=(const Server &src);
		~Server();
	private:
};