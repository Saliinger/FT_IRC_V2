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

    if (args.empty())
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

    if (args.size() == 1)
    {
        // If only channel name provided (args.size() == 1)
        // Should send: RPL_CHANNELMODEIS (324)
        // Format: ":server 324 nick #channel +itkl 50 password"
        std::string mode("+");
        if (channel->getChannelMode(MODE_I))
            mode += "i";
        if (channel->getChannelMode(MODE_T))
            mode += "t";
        if (channel->getChannelMode(MODE_K))
            mode += "k";
        if (channel->getChannelMode(MODE_O))
            mode += "o";
        if (channel->getChannelMode(MODE_L))
            mode += "l";

        if (channel->getChannelMode(MODE_K))
            mode += " " + channel->getPassword();
        if (channel->getChannelMode(MODE_L))
        {
            std::ostringstream oss;
            oss << channel->getClientLimit();
            mode += " " + oss.str();
        }

        client.sendMessage(formatReply(RPL_CHANNELMODEIS, client.getNickname(), channelName + " " + mode));
        return;
    }

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
            {
                std::string modeChange = ":" + client.getNickname() + "!~" + client.getUsername() + "@ft_irc MODE " +
                                         channelName + " " + (action ? "+" : "-") + "i\r\n";
                channel->sendMessageToClients(-1, modeChange);
            }
            break;
        case 't':
            channel->setChannelMode(MODE_T, action);
            {
                std::string modeChange = ":" + client.getNickname() + "!~" + client.getUsername() + "@ft_irc MODE " +
                                         channelName + " " + (action ? "+" : "-") + "t\r\n";
                channel->sendMessageToClients(-1, modeChange);
            }
            break;
        case 'k':
            if (action)
            {
                if (itArgs != args.end())
                {
                    channel->setPassword(*itArgs);
                    channel->setChannelMode(MODE_K, action);
                    {
                        std::string modeChange = ":" + client.getNickname() + "!~" + client.getUsername() +
                                                 "@ft_irc MODE " + channelName + " +k\r\n";
                        channel->sendMessageToClients(-1, modeChange);
                    }
                    itArgs++;
                }
                else
                {
                    client.sendMessage(
                        formatError(ERR_NEEDMOREPARAMS, client.getNickname(), "MODE", "Not enough parameters"));
                }
            }
            else
            {
                channel->setPassword("");
                channel->setChannelMode(MODE_K, action);
                {
                    std::string modeChange = ":" + client.getNickname() + "!~" + client.getUsername() +
                                             "@ft_irc MODE " + channelName + " -k\r\n";
                    channel->sendMessageToClients(-1, modeChange);
                }
            }
            break;
        case 'o':
            if (itArgs != args.end())
            {
                Client *target = server.getClient(*itArgs);
                if (!target)
                {
                    client.sendMessage(
                        formatError(ERR_NOSUCHNICK, client.getNickname(), *itArgs, "No such nick/channel"));
                }
                else if (!channel->isClient(target))
                {
                    client.sendMessage(formatError(ERR_USERNOTINCHANNEL, client.getNickname(),
                                                   *itArgs + " " + channelName, "They aren't on that channel"));
                }
                else
                {
                    if (action)
                        channel->setOperator(target);
                    else
                        channel->removeOperator(target);

                    // Broadcast mode change to all channel members
                    std::string modeChange = ":" + client.getNickname() + "!~" + client.getUsername() +
                                             "@ft_irc MODE " + channelName + " " + (action ? "+" : "-") + "o " +
                                             target->getNickname() + "\r\n";
                    channel->sendMessageToClients(-1, modeChange);
                }
                itArgs++;
            }
            else
            {
                client.sendMessage(
                    formatError(ERR_NEEDMOREPARAMS, client.getNickname(), "MODE", "Not enough parameters"));
            }
            break;
        case 'l':
            if (action)
            {
                if (itArgs != args.end())
                {
                    if (isStringNumeric(*itArgs))
                    {
                        int limit = atol((*itArgs).c_str());
                        if (limit > 0)
                        {
                            channel->setClientLimit(limit);
                            channel->setChannelMode(MODE_L, action);
                            {
                                std::string modeChange = ":" + client.getNickname() + "!~" + client.getUsername() +
                                                         "@ft_irc MODE " + channelName + " +l " + *itArgs + "\r\n";
                                channel->sendMessageToClients(-1, modeChange);
                            }
                        }
                        itArgs++;
                    }
                }
                else
                {
                    client.sendMessage(
                        formatError(ERR_NEEDMOREPARAMS, client.getNickname(), "MODE", "Not enough parameters"));
                }
            }
            else
            {
                channel->setClientLimit(0);
                channel->setChannelMode(MODE_L, action);
                {
                    std::string modeChange = ":" + client.getNickname() + "!~" + client.getUsername() +
                                             "@ft_irc MODE " + channelName + " -l\r\n";
                    channel->sendMessageToClients(-1, modeChange);
                }
            }
            break;

        default:
            client.sendMessage(":ft_irc 472 " + client.getNickname() + " " + std::string(1, *it) +
                               " :is unknown mode char to me\r\n");
            break;
        }
    }
}
