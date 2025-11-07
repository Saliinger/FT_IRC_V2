#pragma once

#include <iostream>
#include <istream>
#include <string>
#include <map>
#include <exception>
#include <poll.h>
#include <fcntl.h>
#include <csignal>

#include "Client.hpp"
#include "Channel.hpp"
#include "Command.hpp"
#include "CommandHandler.hpp"
#include "Error.hpp"
#include "Utils.hpp"

class Server
{
public:
	Server();
	Server(const Server &src);
	Server &operator=(const Server &src);
	~Server();

	Server(std::string pass, int port);

	// ServerNetwork
	void run();
	void acceptClient();
	void recData(const int &fd);

	// signal command
	static void signalHandler(int sig);
	void setupSignalHandlers();
	int getSig();

	bool	isNicknameUsed(const std::string &nick) const;
	const std::string	&getPassword() const;

private:
	static Server *_instance;
	std::map<int, Client *> _clients;
	std::map<std::string, Channel *> _channels;
	std::string _pass;
	int _port;
	std::vector<pollfd> _pollfds;
	int _server_fd;
	bool _isRunning;
	int _sig;
	CommandHandler _commandHandler;
};

// add check nickname 
// getPass();