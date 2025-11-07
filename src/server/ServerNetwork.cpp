// # Socket/connection handling
#include "../../include/Server.hpp"

void Server::run()
{
	while (_isRunning)
	{
		if (poll(&_pollfds[0], _pollfds.size(), -1))
			throw std::runtime_error("Error: poll failed.");
		for (int i = 0; i < _pollfds.size(); i++)
		{
			if (_pollfds[i].fd == _server_fd)
				acceptClient();
			else if (_pollfds[i].revents & POLLIN)
				recData(_pollfds[i].fd);
			else if (_pollfds[i].revents & (POLLERR | POLLHUP))
				std::cout << "Hang up" << std::endl;
		}
	}
}

void Server::acceptClient()
{
	_clients_fd = accept(_server_fd, )
}


void Server::recData(const int &fd)
{
}