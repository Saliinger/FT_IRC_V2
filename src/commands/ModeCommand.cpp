#include "../../include/ModeCommand.hpp"

void ModeCommand::execute(Server &server, Client &client, const std::vector<std::string> &args)
{

	std::string channelName = args[0];
	std::string mode = args[1];

	Channel *channel = server.getChannel(channelName);

	bool action;
	size_t modeParameter = 2;
	for (std::string::iterator it = mode.begin(); it != mode.end(); it++)
	{
		switch (*it)
		{
		// enable / disable
		case '-':
			action = false;
			break;
		case '+':
			action = true;
			break;

		// mode
		case 'i':
			break;
		case 't':
			break;
		case 'k':
			break;
		case 'o':
			break;
		case 'l':
			break;

		default:
			std::cout << "doesn't exist" << std::endl;
			break;
		}
	}
}