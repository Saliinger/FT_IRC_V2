#include "../../include/ModeCommand.hpp"

bool	isStringNumeric(std::string str)
{
	if (str.empty())
		return (false);
	for (int i = 0; str[i]; i++)
	{
		if (!isdigit(str[i]))
			return (false);
	}
	return (true);
}

void ModeCommand::execute(Server &server, Client &client, const std::vector<std::string> &args)
{

	std::string channelName = args[0];
	std::string mode = args[1];

	Channel *channel = server.getChannel(channelName);

	bool action = true;
	std::vector<std::string>::const_iterator itArgs = args.begin() + 2;

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
			channel->setChannelMode(MODE_I, action);
			break;
		case 't':
			channel->setChannelMode(MODE_T, action);
			break;
		case 'k':
			if (action)
			{
				if (itArgs != args.end())
				{
					channel->setPassword(*itArgs);
					itArgs++;
				}
			}
			else
				channel->setPassword("");
			channel->setChannelMode(MODE_K, action);
			break;
		case 'o':
			if (itArgs != args.end())
			{
				Client *target = server.getClient(*itArgs);
				if (target)
				{
					if (action)
						channel->setOperator(target);
					else
						channel->removeOperator(target);
				}
				itArgs++;
			}
			break;
		case 'l':
			if (action)
			{
				if (itArgs != args.end())
				{
					if (isStringNumeric(*itArgs))
					{
						channel->setClientLimit(atol(*itArgs));
						itArgs++;
					}
				}
			}
			else
				channel->setClientLimit(0);
			channel->setChannelMode(MODE_L, action);
			break;

		default:
			std::cout << "doesn't exist" << std::endl;
			break;
		}
	}
}
