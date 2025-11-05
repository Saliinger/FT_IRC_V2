#include "../include/Utils.hpp"

std::vector<std::string> split(const std::string &str, char delimiter)
{
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;

    while (std::getline(ss, token, delimiter))
    {
        if (!token.empty())
            tokens.push_back(token);
    }

    return tokens;
}

void sendToClient(int fd, const std::string &msg)
{
    ssize_t bytes = send(fd, msg.c_str(), msg.size(), 0);
    if (bytes == -1)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            // Buffer full - message dropped (could implement queuing here)
            std::cerr << "Warning: send buffer full for fd " << fd << ", message dropped" << std::endl;
        }
        else
        {
            std::cerr << "send failed for fd " << fd << ": " << strerror(errno) << std::endl;
        }
    }
}
