#include "../../include/PrivmsgCommand.hpp"

static int getType(const std::string &to_check)
{
    if (!to_check.empty() && to_check[0] == '#')
        return 1;
    return 0;
}

void PrivmsgCommand::execute(Server &server, Client &client, const std::vector<std::string> &args)
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
        client.sendMessage(":ft_irc 461 " + client.getNickname() + " PRIVMSG :Not enough parameters\r\n");
        return;
    }

    std::string target_name = args[0];
    std::string msg = args[1];
    std::string fullMsg = ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getIpAdress() +
                          " PRIVMSG " + target_name + " :" + msg + "\r\n";

    if (getType(target_name)) // type 1 channel
    {
        Channel *target = server.getChannel(target_name);
        if (!target)
        {
            client.sendMessage(":ft_irc 403 " + client.getNickname() + " " + target_name + " :No such channel\r\n");
            return;
        }
        target->sendMessageToClients(client.getFd(), fullMsg);
    }
    else // autre type msg vers nickname
    {
        Client *target = server.getClient(target_name);
        if (!target)
        {
            client.sendMessage(":ft_irc 401 " + client.getNickname() + " " + target_name +
                               " :No such nick/channel\r\n");
            return;
        }
        target->sendMessage(fullMsg);
    }
}

// check si # -> channel sinon nickname

// if channel add @ if target is op
