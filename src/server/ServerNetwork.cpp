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
            if (_pollfds[i].revents & POLLIN)
            {
                if (_pollfds[i].fd == _server_fd)
                    acceptClient();
                else
                    recData(_pollfds[i].fd);
            }
            else if (_pollfds[i].revents & (POLLERR | POLLHUP))
            {
                std::cout << "Client disconnected: fd " << _pollfds[i].fd << std::endl;
                std::map<int, Client *>::iterator it = _clients.find(_pollfds[i].fd);
                if (it != _clients.end())
                {
                    Client *client = it->second;
                    // Remove client from all channels
                    const std::map<std::string, Channel *> &channels = client->getChannelList();
                    for (std::map<std::string, Channel *>::const_iterator chIt = channels.begin();
                         chIt != channels.end(); ++chIt)
                    {
                        chIt->second->removeClient(client);
                    }
                    close(_pollfds[i].fd);
                    delete client;
                    _clients.erase(it);
                    _pollfds.erase(_pollfds.begin() + i);
                    --i; // Adjust index after erase
                }
            }
        }
    }
    std::cout << "Server stopped." << std::endl;
}

void Server::acceptClient()
{
    int _client_fd = accept(_server_fd, NULL, NULL);
    if (_client_fd < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
            return; // No connection waiting, just return
        throw std::runtime_error("Error: client connection failed.");
    }

    fcntl(_client_fd, F_SETFL, O_NONBLOCK); // Set client socket to non-blocking

    _clients[_client_fd] = new Client(_client_fd);

    // add client to poll to listen from
    _pollfds.push_back((pollfd){_client_fd, POLLIN, 0});

    std::cout << "New client connected: fd " << _client_fd << std::endl;
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

    while (target->hasCompleteMessage())
    {
        std::string msg = target->extractMessage();
        std::cout << msg << std::endl; // DEBUG

        // Handle CAP directly
        if (msg.find("CAP LS") == 0 || msg.find("CAP LS ") == 0)
        {
            sendToClient(fd, ":ft_irc CAP * LS :\r\n");
        }
        else if (msg.find("CAP END") == 0)
        {
            // CAP void not suported
        }
        else
        {
            _commandHandler->handleCommand(*this, *target, msg);
        }
    }
}
