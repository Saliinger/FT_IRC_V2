#include "../../include/ModeCommand.hpp"

bool isStringNumeric(std::string str)
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
    std::string clientName = client.getNickname();
    if (!client.isRegistered())
    {
        client.sendMessage(
            formatError(ERR_NOTREGISTERED, clientName.empty() ? "*" : clientName, "", "you're not registered"));
        return;
    }

    if (args.size() < 2)
    {
        client.sendMessage(formatError(ERR_NEEDMOREPARAMS, client.getNickname(), "MODE", "Not enough parameters"));
        return;
    }

   

    std::string channelName = args[0];
    std::string mode = args[1];

    Channel *channel = server.getChannel(channelName);

    if (!channel)
    {
        client.sendMessage(formatError(ERR_NOSUCHCHANNEL, client.getNickname(), channelName, "channel doesn't exist"));
        return;
    }

    if (!channel->isClient(&client))
    {
        client.sendMessage(formatError(ERR_NOTONCHANNEL, client.getNickname(), channelName, "not part of the channel"));
        return;
    }

    if (!channel->isOperator(&client))
    {
        client.sendMessage(formatError(ERR_CHANOPRIVSNEEDED, client.getNickname(), channelName, "need to be operator"));
        return;
    }

	 std::cout << "DEBUG ARGS:" << std::endl;
    for (std::vector<std::string>::const_iterator it = args.begin(); it != args.end(); it++)
    {
        std::cout << *it << std::endl;
    }
    std::cout << "DEBUG END" << std::endl;

    bool action = true;
    std::vector<std::string>::const_iterator itArgs = args.begin() + 2;

    for (std::string::iterator it = mode.begin(); it != mode.end(); it++)
    {
        // std::cout << "DEBUG: " << *it << std::endl;
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
                // no pass provided error
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
                // no target error
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
                        channel->setClientLimit(atol((*itArgs).c_str()));
                        itArgs++;
                    }
                }
                // no limit set error
            }
            else
                channel->setClientLimit(0);
            channel->setChannelMode(MODE_L, action);
            break;

        default:
            std::cout << "doesn't exist" << std::endl;
            // mode doesn't exist error
            break;
        }
    }
}
