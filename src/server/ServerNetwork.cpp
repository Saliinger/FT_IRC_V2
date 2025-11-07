// # Socket/connection handling
#include "../../include/Server.hpp"

void Server::run()
{
	_isRunning = true;
	setupSignalHandlers();

	while (_isRunning)
	{
		if (poll(&_pollfds[0], _pollfds.size(), -1) < 0)
		{
			if (_isRunning) // Only throw if not interrupted by signal
				throw std::runtime_error("Error: poll failed.");
			break;
		}
		for (size_t i = 0; i < _pollfds.size(); i++)
		{
			if (_pollfds[i].fd == _server_fd)
				acceptClient();
			else if (_pollfds[i].revents & POLLIN)
				recData(_pollfds[i].fd);
			else if (_pollfds[i].revents & (POLLERR | POLLHUP))
				std::cout << "Hang up" << std::endl; // need to close fd. keep client in case reconnect
		}
	}
	std::cout << "Server stopped." << std::endl;
}

void Server::acceptClient()
{
	int _client_fd = accept(_server_fd, NULL, NULL);
	if (_client_fd < 0)
		throw std::runtime_error("Error: cliant connection failed.");

	_clients[_client_fd] = new Client(_client_fd);

	// add client to poll to listen from
	_pollfds.push_back((pollfd){
		_client_fd,
		POLLIN,
		0});

	// renvoyer la connection
	// repondre au CAP_LS
}

void Server::recData(const int &fd)
{
	Client *target = _clients.find(fd)->second;
	if (!target)
	{
		throw std::runtime_error("Error: client does not exist.");
		return;
	}

	char buffer[1024];

	int bytes_read = -1;
	while ((bytes_read = recv(fd, &buffer, sizeof(buffer), 0)))
	{
		if (bytes_read < 0)
			break;
		buffer[bytes_read] = '\0';
		std::string to_add(buffer);
		target->appendToRecvBuffer(to_add);
	}

	if (target->hasCompleteMessage())
		std::cout << target->extractMessage() << std::endl; // redirect to command
}