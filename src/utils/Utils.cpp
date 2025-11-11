#include "../include/Utils.hpp"

std::vector<std::string> split(const std::string &str, char delimiter)
{
	std::vector<std::string> tokens;
	std::stringstream ss(str);
	std::string token;

	while (std::getline(ss, token, delimiter))
	{
		if (!token.empty() && token[0] == ':')
		{
			// Add the rest of the string (without the ':') as one token
			std::string remaining = token.substr(1);
			std::string rest;
			if (std::getline(ss, rest))
			{
				remaining += delimiter + rest;
				// Get everything else remaining
				while (std::getline(ss, rest))
					remaining += delimiter + rest;
			}
			tokens.push_back(remaining);
			break;
		}
		else if (!token.empty())
		{
			tokens.push_back(token);
		}
	}

	return tokens;
}

void sendToClient(int fd, const std::string &msg)
{
	std::string to_send(msg + "\r\n");
	ssize_t bytes = send(fd, to_send.c_str(), to_send.size(), 0);
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


// old split
// std::vector<std::string> split(const std::string &str, char delimiter)
// {
//     std::vector<std::string> tokens;
//     std::stringstream ss(str);
//     std::string token;

//     while (std::getline(ss, token, delimiter))
//     {
//         if (!token.empty())
//             tokens.push_back(token);
//     }

//     return tokens;
// }
