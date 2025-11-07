#include "../../include/Server.hpp"

Server::Server() : _clients(), _channels(), _isRunning(false) {}

Server::Server(const Server &src) : _clients(src._clients), _channels(src._channels), _isRunning(src._isRunning), _sig(src._sig), _commandHandler(src._commandHandler) {}

Server &Server::operator=(const Server &src)
{
	if (this != &src)
	{
		_clients = src._clients;
		_channels = src._channels;
		_pass = src._pass;
		_port = src._port;
		_pollfds = src._pollfds;
		_server_fd = src._server_fd;
		_isRunning = src._isRunning;
		_sig = src._sig;
		_commandHandler = src._commandHandler;
	}
	return *this;
}

Server::~Server()
{
	// delet each client and channel
}

Server::Server(std::string pass, int port) : _pass(pass), _port(port)
{
	_server_fd = socket(AF_INET, SOCK_STREAM, 0); // open a socket for this program
	if (_server_fd < 0)
		throw std::runtime_error("Error: socket failed.");

	int opt = 1;
	setsockopt(_server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); // block socket for this program

	sockaddr_in addr;

	addr.sin_family = AF_INET;		   // setup for ipv4 can use AF_INET6 for ipv6
	addr.sin_port = htons(_port);	   // host on port
	addr.sin_addr.s_addr = INADDR_ANY; // open to all address incomming connection

	if (bind(_server_fd, (sockaddr *)&addr, sizeof(addr)) < 0)
		throw std::runtime_error("Error: bind failed.");
	if (listen(_server_fd, SOMAXCONN) < 0)
		throw std::runtime_error("Error: listen failed.");

	fcntl(_server_fd, F_SETFL, O_NONBLOCK); // set to non blocking means that we don't wait for a respond

	std::cout << "FT_IRC\nPort: " << _port << "\nPassword: " << _pass << std::endl;
	_pollfds.push_back((pollfd){
		_server_fd,
		POLLIN,
		0});
	_sig = 0;
}


bool	Server::isNicknameUsed(const std::string &nick) const
{
	for (std::map<int, Client *>::const_iterator it = _clients.begin(); it != _clients.end(); ++it)
	{
		Client *client = it->second;
		if (client && client->getNickname() == nick)
			return (true);
	}
	return (false);
}

const std::string	&Server::getPassword() const
{ return (_pass); }