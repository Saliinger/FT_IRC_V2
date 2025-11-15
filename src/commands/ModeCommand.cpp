#include "../../include/ModeCommand.hpp"

void ModeCommand::execute(Server &server, Client &client, const std::vector<std::string> &args)
{

	std::string channelName = args[0];
	std::string mode = args[1];

	bool action;
	for (std::string::iterator it = mode.begin(); it != mode.end; it++)
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

	for (std::vector<std::string>::const_iterator it = args.begin() + 2; it != args.end(); it++)
	{
	}
}