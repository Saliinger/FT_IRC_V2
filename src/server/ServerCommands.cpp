// # Signal command
#include "../../include/Server.hpp"

// Initialize static member
Server *Server::_instance = NULL;

void Server::signalHandler(int sig)
{
	if (_instance)
	{
		_instance->_sig = sig;
		std::cout << "\nShutting down server gracefully..." << std::endl;
		_instance->_isRunning = false;
	}
}

void Server::setupSignalHandlers()
{
	_instance = this;
	signal(SIGINT, signalHandler);
	signal(SIGTERM, signalHandler);
	signal(SIGQUIT, signalHandler);
}

int Server::getSig()
{
	return _sig;
}